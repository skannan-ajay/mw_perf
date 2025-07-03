#pragma once

#include "VNTable.hpp"
#include <llvm/IR/Value.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Constants.h>
#include <llvm/ADT/Hashing.h>
#include <llvm/ADT/DenseMapInfo.h>
#include <unordered_set>

class LLVMValueNumbering {
public:
    using ExprValueTable = VNTable<llvm::Value*>;
    using ValueNumber = ExprValueTable::ValueNumber;

private:
    ExprValueTable fTable;
    ValueNumber fNextVN = 1;
    mutable std::unordered_set<llvm::Value*> fComputingStack; // Track values currently being computed

public:
    // Get or compute value number for an LLVM expression
    ValueNumber getValueNumber(llvm::Value* expr) {
        auto existing = fTable.value(expr);
        if (existing) {
            return *existing;
        }

        // Check for recursion - if we're already computing this value, assign a temporary VN
        if (fComputingStack.find(expr) != fComputingStack.end()) {
            // Return a temporary value number to break recursion
            ValueNumber tempVN = fNextVN++;
            fTable.insertOrReplace(expr, tempVN);
            return tempVN;
        }

        // Mark this expression as being computed
        fComputingStack.insert(expr);
        
        // Compute new value number
        ValueNumber vn = computeExpressionValueNumber(expr);
        fTable.insertOrReplace(expr, vn);
        
        // Remove from computing stack
        fComputingStack.erase(expr);
        
        return vn;
    }

    // Get all expressions with the same value number (congruence class)
    auto getCongruenceClass(ValueNumber vn) const {
        return fTable.congruence(vn);
    }

    // Clear all mappings
    void clear() {
        fTable.clear();
        fNextVN = 1;
    }

private:
    ValueNumber computeExpressionValueNumber(llvm::Value* expr) {
        // Handle different types of LLVM values
        
        if (auto* constant = llvm::dyn_cast<llvm::Constant>(expr)) {
            return computeConstantVN(constant);
        }
        
        if (auto* inst = llvm::dyn_cast<llvm::Instruction>(expr)) {
            return computeInstructionVN(inst);
        }
        
        if (auto* arg = llvm::dyn_cast<llvm::Argument>(expr)) {
            return computeArgumentVN(arg);
        }
        
        // Default: assign unique value number
        return fNextVN++;
    }

    ValueNumber computeConstantVN(llvm::Constant* constant) {
        // Constants with same value should have same VN
        // Use LLVM's built-in constant uniquing
        return reinterpret_cast<ValueNumber>(constant) % 1000000;
    }

    ValueNumber computeInstructionVN(llvm::Instruction* inst) {
        // Compute hash based on opcode and operand value numbers
        llvm::hash_code hash = llvm::hash_value(inst->getOpcode());
        
        // Include operand value numbers in hash
        for (auto& operand : inst->operands()) {
            ValueNumber operandVN = getValueNumber(operand.get());
            hash = llvm::hash_combine(hash, operandVN);
        }
        
        // Also consider instruction attributes for precise matching
        if (auto* binOp = llvm::dyn_cast<llvm::BinaryOperator>(inst)) {
            // Include commutativity for binary operations
            if (binOp->isCommutative()) {
                // Ensure commutative operations get same hash regardless of operand order
                ValueNumber op1VN = getValueNumber(inst->getOperand(0));
                ValueNumber op2VN = getValueNumber(inst->getOperand(1));
                if (op1VN > op2VN) std::swap(op1VN, op2VN);
                hash = llvm::hash_combine(llvm::hash_value(inst->getOpcode()), op1VN, op2VN);
            }
        }
        
        return static_cast<ValueNumber>(hash);
    }

    ValueNumber computeArgumentVN(llvm::Argument* arg) {
        // Arguments get unique value numbers based on their position
        // and parent function
        llvm::hash_code hash = llvm::hash_combine(
            reinterpret_cast<uintptr_t>(arg->getParent()),
            arg->getArgNo()
        );
        return static_cast<ValueNumber>(hash);
    }
};

// Utility functions for common operations
namespace LLVMValueNumberingUtils {
    
    // Check if two expressions are congruent (have same value number)
    bool areCongruent(LLVMValueNumbering& vn, llvm::Value* expr1, llvm::Value* expr2) {
        return vn.getValueNumber(expr1) == vn.getValueNumber(expr2);
    }
    
    // Find all expressions congruent to a given expression
    std::vector<llvm::Value*> findCongruentExpressions(
        LLVMValueNumbering& vn, 
        llvm::Value* expr
    ) {
        auto valueNumber = vn.getValueNumber(expr);
        auto range = vn.getCongruenceClass(valueNumber);
        
        std::vector<llvm::Value*> result;
        for (auto it = range.first; it != range.second; ++it) {
            result.push_back(it->second);
        }
        return result;
    }
}

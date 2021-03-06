// Copyright 2015, Aaron Ceross
#include <vector>
#include <iostream>
#include "../include/variable_manager.hpp"

VariableManager::VariableManager()
: variable_list_()
, clauses_()
, locations_()
, num_vars_()
, num_clauses_()
{}

void VariableManager::LoadVariables(SAT_Manager SAT_manager) {
  num_vars_ = SAT_NumVariables(SAT_manager);
  num_clauses_ = SAT_NumClauses(SAT_manager);

  for (int i = 0; i < num_vars_; ++i) {
    variable_list_.push_back(VariableObject());
    variable_list_[i].SetInitialValue(i);
  }
}

void VariableManager::SetLiteralLocations(int true_literal) {}

bool VariableManager::IsUniqueLit(int true_literal) {
  for (int i = 0; i < clauses_.size(); ++i) {
    for (int j = 0; j < clauses_[i].size(); ++j) {
      if (true_literal == clauses_[i][j]) {
        return true;
      } else {
        return false;
      }
    }
  }
  return false;
}

void VariableManager::SetNumVariables(int num_var) {
  num_vars_ = num_var;
}

unsigned int VariableManager::GetNumClauses() {
  return num_clauses_;
}

unsigned int VariableManager::GetNumVariables() {
  return num_vars_;
}

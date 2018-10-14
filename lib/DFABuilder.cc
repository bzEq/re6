// Copyright (c) 2018 Kai Luo <gluokai@gmail.com>. All rights reserved.

#include "re6/DFABuilder.h"
#include "re6/xxhash.h"

#include <algorithm>
#include <iostream>
#include <queue>
#include <set>

namespace re6 {

namespace {

std::set<char> CollectJumpCharacters(const std::set<NFAState *> &nfa_set) {
  std::set<char> result;
  for (auto nfa : nfa_set) {
    for (auto item : nfa->jump) {
      if (item.first != EP) {
        result.insert(item.first);
      }
    }
  }
  return result;
}

std::set<NFAState *> FindEClosure(NFAState *nfa) {
  std::set<NFAState *> result;
  std::queue<NFAState *> worklist;
  result.insert(nfa);
  worklist.push(nfa);
  while (not worklist.empty()) {
    NFAState *active_nfa = worklist.front();
    worklist.pop();
    if (active_nfa->jump.count(EP)) {
      std::set<NFAState *> &targets = active_nfa->jump[EP];
      for (auto t : targets) {
        if (not result.count(t)) {
          result.insert(t);
          worklist.push(t);
        }
      }
    }
  }
  return result;
}

std::set<NFAState *> FindJumpTargets(char c,
                                     const std::set<NFAState *> &nfa_set) {
  std::set<NFAState *> result;
  for (auto nfa : nfa_set) {
    if (nfa->jump.count(c)) {
      result.insert(nfa->jump[c].begin(), nfa->jump[c].end());
    }
  }
  return result;
}

} // namespace

void DFABuilder::Build() {
  std::set<NFAState *> active_nfa_set;
  auto e_closure = FindEClosure(nfa_start_);
  active_nfa_set.insert(e_closure.begin(), e_closure.end());
  result_.start = GetOrCreateState(active_nfa_set);
  std::queue<std::set<NFAState *>> worklist;
  worklist.push(std::move(active_nfa_set));
  std::set<DFAState *> done_set;
  while (not worklist.empty()) {
    active_nfa_set = std::move(worklist.front());
    worklist.pop();
    DFAState *dfa_state = GetOrCreateState(active_nfa_set);
    if (done_set.count(dfa_state)) {
      continue;
    }
    // std::cout << "Visiting DFA state: " << dfa_state << "\n";
    // std::cout << "NFA set size: " << active_nfa_set.size() << "\n";
    std::set<char> char_set(CollectJumpCharacters(active_nfa_set));
    for (char c : char_set) {
      // std::cout << "jump for char: " << c << "\n";
      std::set<NFAState *> targets = FindJumpTargets(c, active_nfa_set);
      std::set<NFAState *> with_closure;
      std::for_each(targets.begin(), targets.end(),
                    [&with_closure](NFAState *nfa) {
                      auto closure = FindEClosure(nfa);
                      with_closure.insert(closure.begin(), closure.end());
                    });
      DFAState *c_target = GetOrCreateState(with_closure);
      dfa_state->SetJump(c, c_target);
      worklist.push(with_closure);
    }
    if (active_nfa_set.count(nfa_finish_)) {
      result_.finish_set.insert(dfa_state);
    }
    done_set.insert(dfa_state);
  }
}

DFAState *DFABuilder::GetOrCreateState(uint64_t hash) {
  auto it = states_map_.find(hash);
  if (it != states_map_.end()) {
    return it->second;
  }
  DFAState *state = alloc_->Create();
  states_map_.insert(std::make_pair(hash, state));
  return state;
}

uint64_t DFABuilder::EncodeNFASet(const std::set<NFAState *> &nfa_set) {
  std::string temp;
  for (auto p : nfa_set) {
    temp.append(reinterpret_cast<const char *>(p), sizeof(p));
  }
  return XXH64(temp.data(), temp.size(), 0);
}

DFAState *DFABuilder::GetOrCreateState(const std::set<NFAState *> &nfa_set) {
  return GetOrCreateState(EncodeNFASet(nfa_set));
}

} // namespace re6

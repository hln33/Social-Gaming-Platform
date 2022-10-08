#pragma once

// Description: struct that provides typed wrapper for arguments, taken from CMPT373 in-class example 
template <typename Base, typename Tag>
struct StrongAlias {
  explicit StrongAlias(Base value) : value{std::move(value)} { }
  Base value;
};
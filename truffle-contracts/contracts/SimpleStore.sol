pragma solidity ^0.5.17;
contract SimpleStore {

  event Set(uint _value);
  
  function set(uint _value) public {
    value = _value;
    emit Set(_value);
  }

  function get() public view returns (uint) {
    return value;
  }

  uint value;
}
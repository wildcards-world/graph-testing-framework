const SimpleStore = artifacts.require("./SimpleStore.sol");

module.exports = async function(deployer) {
  await deployer.deploy(SimpleStore);
  let store = await SimpleStore.deployed();
  await store.set(10);
  await store.set(12);
  await store.set(14);
  await store.set(16);
};


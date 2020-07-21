const SimpleStore = artifacts.require("./SimpleStore.sol");

module.exports = async function(deployer, networkName) {
  await deployer.deploy(SimpleStore);
  let store = await SimpleStore.deployed();

  let isLocalTestnet = (networkName === "test");
  console.log("   > Integration-Testing Mode: " + isLocalTestnet)

  if(isLocalTestnet) {
    await store.set(10);
    await store.set(12);
    await store.set(14);
    await store.set(16);
  }  
};


const SimpleStore = artifacts.require("./SimpleStore.sol");

contract("First Test", async () => {
    let store;

    beforeEach(async () => {
        store = await SimpleStore.deployed();
    });

    it("should be 16", async () => {
        let value = await store.get();
        assert.equal(value, 16);
    });

    
    it("should be 20", async () => {
        await store.set(20);
        let value = await store.get();
        assert.equal(value, 20);
    });

    it("should revert because of wrong input", async () => {
        let error;
        try {
            await store.set("hello")
        } catch(e) {
            error = e.code
        }
        assert.equal(error, "INVALID_ARGUMENT");
    }); 

});

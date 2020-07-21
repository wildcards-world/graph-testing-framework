open BsMocha;
// open Async;
let (it', it_skip') = Async.(it, it_skip);
open Mocha;

module SimpleQuery = [%graphql
  {|
    query SimpleQuery {
        simpleStorages {
            id
            count
            _value
        }
    }
|}
];

let exampleQuery = _ =>
  Client.instance
  ->ApolloClient.query(~query=(module SimpleQuery), ())
  ->Js.Promise.then_(
      (result) =>
        // switch (result) {
        // | {data: Some({SimpleQuery.people})} =>
        //   Js.Promise.resolve(Js.log2("Data: ", people))
        // | _ => Js.Exn.raiseError("Error: no people!")
        // },
        {
          Js.log(result) ->Js.Promise.resolve;},
      _,
    )
  ->Js.Promise.catch(
      error => Js.Promise.resolve(Js.log2("Error: ", error)),
      _,
    );

  let simpleAsync = () => Js.Promise.make((~resolve, ~reject as _) => {
    ignore(
      Js.Global.setTimeout(
        () =>
          resolve(.
            "Finished Featching Skale Data - this is some test data that must be pushed to arweave...",
          ),
        100,
      ),
    )
  })

describe("Some Test Suite", () => {
  describe("List.map", () => {
    it("should map the values", () =>
      Assert.deep_equal(List.map(( * )(2), [1, 2, 3]), [2, 4, 6])
    );

    it("should work with an empty list", () =>
      Assert.equal(List.map(( * )(2), []), [])
    );

    it("should work with an empty list Async", ~timeout=5000, () =>{
      Js.Promise.then_(
      (_stringReturn) => Assert.equal(List.map(( * )(2), []), [])->Js.Promise.resolve
        ,simpleAsync())->ignore;
    }
    );

    it("Async - Query", ~timeout=5000, () =>
       { exampleQuery() -> Js.Promise.then_(() => Assert.equal(List.map(( * )(2), []), []) ->Js.Promise.resolve, _)->ignore; }
    );
  })
});
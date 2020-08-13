/*
open BsMocha;
let (describe, describe_skip) = Mocha.(describe, describe_skip);
open Promise;

exception RejectedError(string);

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

// type StateChange @entity {
//   id: ID! # tx
//   timestamp: BigInt!
//   txEventList: [String!]! #call event logs or something
//   patronChanges: [Patron!]!
//   wildcardChanges: [Wildcard!]!
// }

module TimeTravelQueryHardcoded = [%graphql
  {|
    query BlockNumberQuery {
        simpleStorages(block: { number: 4 }) {
            id
            count
            _value
        }
    }
|}
];

module TimeTravelQueryParameterized = [%graphql
  {|
    query BlockNumberQuery ($blockNumber: Int!) {
        simpleStorages(block: { number: $blockNumber }) {
            id
            count
            _value
        }
    }
|}
];

let exampleQuery = () =>
  Client.instance
  ->ApolloClient.query(~query=(module SimpleQuery), ())
  ->Js.Promise.then_(
      (result: ApolloClient__ApolloClient.ApolloQueryResult.t(SimpleQuery.t)) =>
        switch (result) {
        | {data: Some(data)} =>
          SimpleQuery.(
            Js.Promise.resolve(
              switch (data.simpleStorages) {
              | [|simpleStorage|]
              | [|simpleStorage, _|] => Some(simpleStorage)
              | _ => None
              },
            )
          )
        | _ => Js.Exn.raiseError("Error: no people!")
        },
      _,
    )
  ->Js.Promise.catch(error => Js.Exn.raiseError(error->Obj.magic), _);

let exampleTimeTravelQueryHardcoded = () => {
  TimeTravelQueryHardcoded.(
    Client.instance
    ->ApolloClient.query(~query=(module TimeTravelQueryHardcoded), ())
    ->Js.Promise.then_(
        (
          result:
            ApolloClient__ApolloClient.ApolloQueryResult.t(
              TimeTravelQueryHardcoded.t,
            ),
        ) =>
          switch (result) {
          | {data: Some(data)} =>
            Js.Promise.resolve(
              switch (data.simpleStorages) {
              | [|simpleStorage|]
              | [|simpleStorage, _|] => Some(simpleStorage)
              | _ => None
              },
            )
          | _ => Js.Exn.raiseError("Error: no people!")
          },
        _,
      )
    ->Js.Promise.catch(error => Js.Exn.raiseError(error->Obj.magic), _)
  );
};

let exampleTimeTravelQueryParameterized = blockNumber => {
  TimeTravelQueryParameterized.(
    Client.instance
    ->ApolloClient.query(
        ~query=(module TimeTravelQueryParameterized),
        TimeTravelQueryParameterized.makeVariables(~blockNumber, ()),
      )
    ->Js.Promise.then_(
        (
          result:
            ApolloClient__ApolloClient.ApolloQueryResult.t(
              TimeTravelQueryParameterized.t,
            ),
        ) =>
          switch (result) {
          | {data: Some(data)} =>
            Js.Promise.resolve(
              switch (data.simpleStorages) {
              | [|simpleStorage|]
              | [|simpleStorage, _|] => Some(simpleStorage)
              | _ => None
              },
            )
          | _ => Js.Exn.raiseError("Error: no response")
          },
        _,
      )
    ->Js.Promise.catch(error => Js.Exn.raiseError(error->Obj.magic), _)
  );
};

describe("Graph Test", () => {
  describe("Fetching set value", () => {
    it("should be successful", () => {
      SimpleQuery.(
        Js.Promise.then_(
          result =>
            (
              switch (result) {
              | None => Assert.equal(true, false)
              | Some(queryResult) =>
                Assert.equal(queryResult._value->Obj.magic, "16")
              }
            )
            ->Js.Promise.resolve,
          exampleQuery(),
        )
      )
    });

    it("should be 10 at block 4", () => {
      TimeTravelQueryHardcoded.(
        Js.Promise.then_(
          result =>
            (
              switch (result) {
              | None => Assert.equal(true, false)
              | Some(queryResult) =>
                Assert.equal(queryResult._value->Obj.magic, "10")
              }
            )
            ->Js.Promise.resolve,
          exampleTimeTravelQueryHardcoded(),
        )
      )
    });
    it("PARAMETERIZED: should be 10 at block 4", () => {
      TimeTravelQueryParameterized.(
        Js.Promise.then_(
          result =>
            (
              switch (result) {
              | None => Assert.equal(true, false)
              | Some(queryResult) =>
                Assert.equal(queryResult._value->Obj.magic, "10")
              }
            )
            ->Js.Promise.resolve,
          exampleTimeTravelQueryParameterized(4),
        )
      )
    });
  })
});
*/

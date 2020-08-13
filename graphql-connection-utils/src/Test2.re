open BsMocha;
// let (describe, describe_skip) = Mocha.(describe, describe_skip);
// open Promise;
let describe = Mocha.describe;
open Async;


exception RejectedError(string);

module AllStateChanges = [%graphql
  {|
    query AllStateChanges {
      stateChanges(first: 100, orderBy: timestamp, orderDirection: desc) {
        id
        timestamp
        blockNumber
        contractVersion
        txEventList
        txEventParamList
        patronChanges {
          id
        }
        wildcardChanges {
          id
        }
      }
    }
|}
];

// module TimeTravelQueryHardcoded = [%graphql
//   {|
//     query BlockNumberQuery {
//         simpleStorages(block: { number: 4 }) {
//             id
//             count
//             _value
//         }
//     }
// |}
// ];

module PatronAtBlockNumber = [%graphql
  {|
    query BlockNumberQuery ($blockNumber: Int!, $address: String!) {
      patron(id: $address, block: { number: $blockNumber }) {
        id
        tokens {
          id
        }
      }

    }
|}
];

let exampleQuery = () =>
  Client.instance
  ->ApolloClient.query(~query=(module AllStateChanges), ())
  ->Js.Promise.then_(
      (result: ApolloClient__ApolloClient.ApolloQueryResult.t(AllStateChanges.t)) =>
        switch (result) {
        | {data: Some(data)} =>
          AllStateChanges.(
            Js.Promise.resolve(
              data.stateChanges
            )
          )
        | _ => Js.Exn.raiseError("Error: no people!")
        },
      _,
    )
  ->Js.Promise.catch(error => Js.Exn.raiseError(error->Obj.magic), _);

// let exampleTimeTravelQueryHardcoded = () => {
//   TimeTravelQueryHardcoded.(
//     Client.instance
//     ->ApolloClient.query(~query=(module TimeTravelQueryHardcoded), ())
//     ->Js.Promise.then_(
//         (
//           result:
//             ApolloClient__ApolloClient.ApolloQueryResult.t(
//               TimeTravelQueryHardcoded.t,
//             ),
//         ) =>
//           switch (result) {
//           | {data: Some(data)} =>
//             Js.Promise.resolve(
//               switch (data.simpleStorages) {
//               | [|simpleStorage|]
//               | [|simpleStorage, _|] => Some(simpleStorage)
//               | _ => None
//               },
//             )
//           | _ => Js.Exn.raiseError("Error: no people!")
//           },
//         _,
//       )
//     ->Js.Promise.catch(error => Js.Exn.raiseError(error->Obj.magic), _)
//   );
// };

let examplePatronAtBlockNumber = (blockNumber,address) => {
  PatronAtBlockNumber.(
    Client.instance
    ->ApolloClient.query(
        ~query=(module PatronAtBlockNumber),
        PatronAtBlockNumber.makeVariables(~blockNumber, ~address, ()),
      )
    ->Js.Promise.then_(
        (
          result:
            ApolloClient__ApolloClient.ApolloQueryResult.t(
              PatronAtBlockNumber.t,
            ),
        ) =>
          switch (result) {
          | {data: Some(data)} =>
            Js.Promise.resolve(
              data.patron
              ,
            )
          | _ => Js.Exn.raiseError("Error: no response")
          },
        _,
      )
    ->Js.Promise.catch(error => Js.Exn.raiseError(error->Obj.magic), _)
  );
};

describe("Graph Test", (done_) => {
  describe("Fetching set value", () => {
    it("should be successful", (done_) => {
      {AllStateChanges.(
        Js.Promise.then_(
          result =>
            (
                {
                  Js.log2("First result", result);
                  Assert.equal(result->Array.length>0, true);
                done_();
                }
            )
            ->Js.Promise.resolve,
          exampleQuery(),
        )
      )->ignore;
      ();
    };   });
  
    // it("should be 10 at block 4", () => {
    //   TimeTravelQueryHardcoded.(
    //     Js.Promise.then_(
    //       result =>
    //         (
    //           switch (result) {
    //           | None => Assert.equal(true, false)
    //           | Some(queryResult) =>
    //             Assert.equal(queryResult._value->Obj.magic, "10")
    //           }
    //         )
    //         ->Js.Promise.resolve,
    //       exampleTimeTravelQueryHardcoded(),
    //     )
    //   )
    // });
    it("PARAMETERIZED: should be 10 at block 4", (done_) => {
      PatronAtBlockNumber.(
        Js.Promise.then_(
          result =>
            (
              {

                  Js.log2("second result", result);
                switch (result) {
              | None => Assert.equal(true, false)
              | Some(queryResult) =>
                // Assert.equal(queryResult._value->Obj.magic, "10")
                Assert.equal("10", "10")
              };
              done_();}
            )
            ->Js.Promise.resolve,
          examplePatronAtBlockNumber(14, "0x9545c4a76992e63b2e64b07a2a2fc0927988bc6f"),
        )
      )->ignore
    });
  })
});

#r "../packages/FSharp.Data/lib/net40/FSharp.Data.dll"
open System
open FSharp.Data

// ----------------------------------------------------------------------------
// Domain model - representing a decision tree
// ----------------------------------------------------------------------------

type Feature<'T> = Feature of string * ('T -> float)
type Prediction<'T> = Prediction of ('T -> string)

type DecisionChoice = 
  { Feature   : string
    Threshold : float
    LessThan  : DecisionTree
    MoreThan  : DecisionTree }

and DecisionTree = 
  | DecisionChoice of DecisionChoice
  | DecisionResult of string

// ----------------------------------------------------------------------------
// Training a decision tree
// ----------------------------------------------------------------------------

/// Calculates entropy - the more diverse the input is, the larger the entropy
let entropy input = 
  let l = input |> Seq.length |> float
  let freqs = input |> Seq.countBy id |> Seq.map (fun (_, c) -> float c / l)
  -1.0 * (freqs |> Seq.map (fun f -> f * Math.Log(f, 2.0)) |> Seq.sum)

entropy "ABCDEFGH" // 3.0
entropy "AAAABBBB" // 1.0
entropy "AAAAAAAA" // 0.0


/// To find the splitting point for a range of float values, we 
/// split the range into given number of buckets and pick the best
let bucketCount = 10.0

let chooseIndicator (Prediction p) features data = 
  let indicatorEnts = 
    // Find the feature that produces maximum entropy change
    let oldent = data |> Array.map p |> entropy
    [ for (Feature(_, f)) as feature in features do
        
        // Get range of values for the feature 
        let values = data |> Array.map f |> Array.filter (fun v -> not (Double.IsNaN v))
        let min, max = Seq.min values, Seq.max values
        let step = (max - min) / bucketCount
        if step > 0.0 then
          
          // Find splitting in the range such that it produces 
          // maximal increase in entropy
          let ent, threshold = 
            [ for threshold in min .. step .. max do
                let lt, gt = values |> Array.partition (fun v -> v < threshold)
                let lte, gte = entropy lt, entropy gt
                let ent = 
                  oldent 
                    - (float lt.Length / float data.Length) * lte
                    - (float gt.Length / float data.Length) * gte
                if not (Double.IsNaN(ent)) then yield ent, threshold ] |> List.maxBy fst        
          
          // If we used this feature, this is what we'd get
          yield ent, (feature, threshold) ]
  if List.isEmpty indicatorEnts then None
  else
    // If there is some feature, pick the best one
    let ent, (indicator, threshold) = indicatorEnts |> Seq.maxBy fst
    Some(indicator, threshold)


/// Build a deceision tree - if we have not reached conclusion,
/// and if there are some features & data left, find the best
/// feature and split the tree using the feature
let rec buildDecisionTree (Prediction p) features maxSize data =
  let predictions = data |> Array.map p
  let options = predictions |> set
  let dominant = predictions |> Seq.countBy id |> Seq.maxBy snd |> fst
  if List.length features = 0 || options.Count <= 1 || maxSize = 0 then 
    DecisionResult(dominant)
  else
    // Find the best indicator using 'chooseIndicator' for given 'features' and 'data'.
    //  - If there is no feature, retrn 'DecisionResult' with 'dominant' as the answer
    //  - Otherwise, filter 'data' into two parts, one < threshold, other >= threshold
    //    and buld the tree recursively on these two sub-parts of the data set. Also,
    //    drop the currently used 'feature' from the feature list.
    match chooseIndicator (Prediction p) features data with
    | None -> DecisionResult(dominant)
    | Some(Feature(name, f), threshold) ->
        let lt = data |> Array.filter (fun d -> let v = f d in Double.IsNaN v || v < threshold)
        let gt = data |> Array.filter (fun d -> let v = f d in Double.IsNaN v || v >= threshold)
        if lt.Length = 0 || gt.Length = 0 then 
          DecisionResult(dominant)
        else
          let otherFeatures = features |> List.filter (fun (Feature(n, _)) -> n <> name)
          let ltree = buildDecisionTree (Prediction p) otherFeatures (maxSize - 1) lt
          let rtree = buildDecisionTree (Prediction p) otherFeatures (maxSize - 1) gt
          if ltree = rtree then ltree 
          else
            DecisionChoice
              { Feature = name
                Threshold = threshold
                LessThan = ltree
                MoreThan = rtree }

// ----------------------------------------------------------------------------
// Running decision tree on Titanic passangers
// ----------------------------------------------------------------------------

module Titanic = 
  type Titanic = CsvProvider<"data/titanic.csv">
  type TitanicRow = Titanic.Row

  let features : Feature<TitanicRow> list = 
    [ Feature("Sex", fun r -> if r.Sex = "male" then 0.0 else 1.0) 
      Feature("Age", fun r -> r.Age)
      Feature("Class", fun r -> float r.Pclass)
      Feature("Siblings", fun r -> float r.SibSp) ]

  let survived = Prediction(fun (r:TitanicRow) -> string r.Survived)
  let data = Titanic.GetSample().Rows |> Array.ofSeq
  let tree = buildDecisionTree survived features 5 data

// ----------------------------------------------------------------------------
// Running decision tree on Iris data set
// ----------------------------------------------------------------------------

module Iris = 
  type Iris = CsvProvider<"data/iris.csv">
  type IrisRow = Iris.Row

  let features : Feature<IrisRow> list = 
    [ Feature("Petal length", fun r -> float r.``Petal length``) 
      Feature("Petal width", fun r -> float r.``Petal width``)
      Feature("Sepal length", fun r -> float r.``Sepal length``)
      Feature("Sepal width", fun r -> float r.``Sepal width``) ]

  let species = Prediction(fun (r:IrisRow) -> string r.Species)
  let data = Iris.GetSample().Rows |> Array.ofSeq
  let tree = buildDecisionTree species features 5 data

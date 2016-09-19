type date = System.DateTime

// decomposing tuples
let fullName = ("James","Kirk")
let (firstName,lastName) = fullName
printfn "%s, %s" lastName firstName

// decomposing records
type Address = 
  { Street: string
    City  : string
    State : string 
    Postal: string }
let homeBase = { Street = "2948 135th Street"
                 City   = "Riverside, Iowa"
                 State  = "IA"
                 Postal = "52327" }
let { Street= street
      City  = city
      State = state 
      Postal= postal} = homeBase
printfn "%s\n%s, %s %s" street city state postal

// decomposing unions
type Pet =
  | Dog of name
  | Cat of name * lives
  | Fish
and name  = string
and lives = int
let printPet = function
  | Dog(name)       -> printfn "%s is a dog" name
  | Cat(name,lives) -> printfn "%s is a cat with %i lives" name lives
  | Fish            -> printfn "Who names fish? They're basically art."
let brook = Cat("Brook",9)
printPet brook

// decomposing lists
let sum items =
  let rec sum' total = function
    | []          -> total
    | head::tail  -> sum' (total + head) tail
  sum' 0 items
let odds = 9::7::5::3::1::[]
sum odds
//NOTE: in reality we'd replace the previous with:
//      List.sum [for i in 1 .. 2 .. 10 -> i]

// decomposing arrays
let check info =
  match info with
  | [| name; dob; |] -> printfn "%s was born on %A" name (date.Parse dob)
  | [| name       |] -> printfn "%s didn't provide a date of birth" name
  | _                -> printfn "Enter a name, and optionally, a dob"
let info = [| "James"; "2233-03-22 19:20" |]
check info
check Array.empty

// inline decomposition
// nested decomposition
type Person =
  { FullName: firstName * lastName
    Address : Address }
and firstName = name
and lastName  = name
let details {FullName = (firstName,_)
             Address  = {City   = city
                         State  = state }} =
  printfn "%s hails from %s, %s" firstName city state
  //NOTE: wildcards may be used to omit tuple items we don't want
  //NOTE: when decomposing a record, we can omit fields we don't want
details { FullName  = fullName 
          Address   = homeBase }

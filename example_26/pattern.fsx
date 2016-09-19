open System
open System.Numerics
open System.Text.RegularExpressions

let rnd = Random DateTime.Now.Millisecond
let items = Map.ofSeq [ for n in  1  .. 26  do
                        for c in 'a' .. 'z' -> (n,c)]

let n = rnd.Next () 
match items.TryFind n with
| Some letter -> printfn "%i = %c" n letter
| None        -> printfn "key not found"

(* ________________________________________________________________________________ *)

for i in 1 .. 100 do
  match i % 3, i % 5 with
  | 0,0 -> printfn "FizzBuzz"
  | 0,_ -> printfn "Fizz"
  | _,0 -> printfn "Buzz"
  | _,_ -> printfn "%s" (string i)

(* ________________________________________________________________________________ *)

// from: [IFCP’07 – Syme, Neverov, Margetson](http://research.microsoft.com/pubs/79947/p29-syme.pdf)

let rec formatType (typ :Type) =
  if typ.IsGenericParameter  then
    sprintf "!%d" typ.GenericParameterPosition
  elif typ.IsGenericType ||
     not typ.HasElementType then
    let args = if typ.IsGenericType
               then typ.GetGenericArguments()
               else [| |]
    let con = typ.GetGenericTypeDefinition()
    if args.Length = 0
    then sprintf "%s" con.Name
    else sprintf "%s<%s>" con.Name (formatTypes args)
  elif typ.IsArray then
    sprintf "Array(%d,%s)"
        (typ.GetArrayRank())
        (formatType (typ.GetElementType()))
  elif typ.IsByRef then
    sprintf "%s&"
        (formatType (typ.GetElementType()))
  elif typ.IsPointer then
    sprintf "%s*"
        (formatType (typ.GetElementType()))
  else failwith "MSDN says this can’t happen"

and formatTypes typs =
  String.Join(",",Array.map formatType typs)

let (|Named|Array|Ptr|Param|) (typ :Type) =
  if   typ.IsGenericType
  then Named (typ.GetGenericTypeDefinition(),typ.GetGenericArguments())
  elif typ.IsGenericParameter
  then Param (typ.GenericParameterPosition)
  elif not typ.HasElementType
  then Named (typ, [| |])
  elif typ.IsArray
  then Array (typ.GetElementType(),typ.GetArrayRank())
  elif typ.IsByRef
  then Ptr (true,typ.GetElementType())
  elif typ.IsPointer
  then Ptr (false,typ.GetElementType())
  else failwith "MSDN says this can’t happen"

let rec format_type typ =
  match typ with
  | Named (con, [||]) ->
      sprintf "%s" con.Name
  | Named (con, args) ->
      sprintf "%s<%s>" con.Name (formatTypes args)
  | Array (arg, rank) ->
      sprintf "Array(%d,%s)" rank (formatType arg)
  | Ptr(true,arg)     ->
      sprintf "%s&"  (formatType arg)
  | Ptr(false,arg)    ->
      sprintf "%s*"  (formatType arg)
  | Param(pos)        ->
      sprintf "!%d" pos

and format_types typs =
  String.Join (",",Array.map formatType typs)

(* ________________________________________________________________________________ *)

let (|Rect|) (x :Complex) = 
  Rect (x.Real,x.Imaginary)

let (|Polar|) (x :Complex) = 
  Polar (x.Magnitude, x.Phase)

let add one two = 
  match one,two with 
  | Rect (r1,i1)
   ,Rect (r2,i2) -> Complex (r1 + r2,i1 + i2)

let mul one two =
  match one,two with 
  | Polar (m1,p1)
   ,Polar (m2,p2) -> Complex (m1 + m2,p1 + p2)

(* ________________________________________________________________________________ *)

let (|Fizz|Buzz|FizzBuzz|Num|) n =
  match n % 3, n % 5 with
  | 0,0 -> FizzBuzz
  | 0,_ -> Fizz
  | _,0 -> Buzz
  | _,_   -> Num n

for i in 1 .. 100 do
  match i with
  | FizzBuzz  -> printfn "FizzBuzz"
  | Fizz      -> printfn "Fizz"
  | Buzz      -> printfn "Buzz"
  | Num n     -> printfn "%s" (string n)
        
(* ________________________________________________________________________________ *)

let (|Fizz|_|) value =
  if value % 3 = 0 then Some () else None

let (|Buzz|_|) value = 
  if value % 5 = 0 then Some () else None
    
for i in 1 .. 100 do
  match i with
  | Fizz & Buzz -> printfn "FizzBuzz"
  | Fizz        -> printfn "Fizz"
  | Buzz        -> printfn "Buzz"
  | n           -> printfn "%s" (string n)

(* ________________________________________________________________________________ *)

let (|Groups|_|) pattern value =
  let m = Regex.Match (value,pattern)
  match m.Success,m.Groups.Count with
  | true,n when n > 0 ->  [ for g in m.Groups -> g.Value ]
                          |> List.tail // drop "root" match
                          |> Some
  | _                 ->  None

match "37206" with
| Groups "(\d{5})([-]\d{4})?" [ zip; _ ] -> printfn "Postal code: %s" zip
| otherwise                              -> printfn "Can't ship to: %s" otherwise

(* ________________________________________________________________________________ *)

type Tree<'T> =
  | Leaf   of 'T
  | Branch of Tree<'T> * Tree<'T>

let (|Branch|_|) = function 
  | Branch (l,r) -> Some (l,r)
  | _            -> None

let collect (|Pred|_|) root =
  let rec loop values node =
    match  node with
    | Pred (v,next) -> loop (v::values) next
    | _             -> List.rev values, root
  loop [] root

let (|Leaves|) root = collect (|Branch|_|) root

let tree = Branch (Branch (Branch (Leaf 2,Leaf 3),Leaf 7),Leaf 1)
match tree with
| Leaves (items,root) -> 
    printfn "%A" root
    for item in items do
      printfn "%A" item


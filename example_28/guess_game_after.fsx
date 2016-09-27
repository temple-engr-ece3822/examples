open System

let rnd = Random DateTime.Now.Millisecond
let scanfn = Console.ReadLine

let (|Int|_|) value =
  match Int32.TryParse value with
  | true ,answer  -> Some answer
  | false,_       -> None

let (|NotIn|_|) (n,m) value =
  if value < n || value > m then Some () else None

let (|Eq|_|) control test =
  if control = test then Some () else None

let guessTheNumber () =
  let correct = rnd.Next (1,10)
  printfn "DEBUG correct answer is: %i" correct

  let rec loop () =
    printf "Guess a number between 1 and 10 (and press <RETURN>, use 0 to exit): "
    match scanfn () with
    | Int answer  ->  match answer with
                      | 0             ->  printfn "Goodbye."
                      | NotIn (1,10)  ->  printfn "Invalid input."
                                          loop ()
                      | Eq correct    ->  printfn "Correct! (%i)" answer
                      | _             ->  printfn "Close... but no cigar :-)"
                                          loop ()
                        
    | _           ->  printfn "Invalid input."
                      loop ()
  loop ()

do guessTheNumber ()

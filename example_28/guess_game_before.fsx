open System

let rnd = Random DateTime.Now.Millisecond
let scanfn = Console.ReadLine

let guessTheNumber () =
  let correct = rnd.Next (1,10)
  printfn "DEBUG correct answer is: %i" correct

  // Can you spot the bug in the following function?
  let rec loop () =
    printf "Guess a number between 1 and 10 (and press <RETURN>, use 0 to exit): "
    match Int32.TryParse (scanfn ()) with
    | true,answer ->  match answer with
                      | 0                       ->  printfn "Goodbye."
                      | n when n < 1 || n > 10  ->  printfn "Invalid input."
                                                    loop ()
                      | c
                        when c = correct        ->  printfn "Correct! (%i = %i)" correct answer
                      | _                       ->  printfn "Close... but no cigar :-)"
                                                    loop ()
    | _           ->  printfn "Invalid input."
                      loop ()
  loop ()

do guessTheNumber ()

namespace Demos
// ----------------------------------------------------------------------------
// Observable 
// ----------------------------------------------------------------------------

type MiniObservable<'T> = 
  abstract Subscribe : ('T -> unit) -> (unit -> unit) -> unit

module MiniObservable = 
  let observable f = 
    { new MiniObservable<'R> with
        member x.Subscribe v d = f v d }

  let ofSeq source = 
    observable (fun onValue onDone ->
      for v in source do onValue v
      onDone () )

  let map (f:'T -> 'R) (source:MiniObservable<'T>) = 
    observable (fun onValue onDone ->
      source.Subscribe
        (fun v -> onValue (f v))
        onDone )

  let subscribe f (source:MiniObservable<'T>) = 
    source.Subscribe f ignore

  let toArrayAsync (source:MiniObservable<'T>) = 
    Async.FromContinuations(fun (cont, _, _) ->
      let res = System.Collections.Concurrent.ConcurrentQueue()
      source.Subscribe
        (fun v -> res.Enqueue(v))
        (fun () -> cont(res.ToArray())) )

namespace Demos
// ----------------------------------------------------------------------------
// Async sequences
// ----------------------------------------------------------------------------

type AsyncSeqResult<'T> = 
  | Nil
  | Cons of 'T * AsyncSeq<'T>

and AsyncSeq<'T> = 
  | AsyncSeq of Async<AsyncSeqResult<'T>>

module AsyncSeq = 
  let rec unfold f initial = AsyncSeq <| async {
    match f initial with 
    | None -> return Nil
    | Some(res, state) -> return Cons(res, unfold f state) }
  
  let rec unfoldAsync (f: 'a -> Async<('b*'a) option>) (inital:'a) = AsyncSeq <| async {
    let! r = f inital
    match r with
    | None -> return Nil
    | Some(res, state) -> return Cons(res, unfoldAsync f state) }

  
  let rec map f (AsyncSeq(a)) = AsyncSeq <| async {
    let! res = a 
    match res with
    | Nil -> return Nil
    | Cons(v, vs) -> return Cons(f v, map f vs) }

  let rec iter f (AsyncSeq(a)) = async {
    let! res = a
    match res with 
    | Nil -> return ()
    | Cons(v, vs) -> 
        f v
        return! iter f vs }


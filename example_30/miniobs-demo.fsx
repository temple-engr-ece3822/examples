#load "miniobs.fs"
open Demos
open System.Net

// Override the default .NET limit for 2 connections
System.Net.ServicePointManager.DefaultConnectionLimit <- 20

let urls = 
  [ "http://www.jet.com"
    "http://www.fsharp.org"
    "http://www.tomasp.net"
    "http://www.fssnip.net"
    "http://www.c4fsharp.net" ]

MiniObservable.ofSeq urls
|> MiniObservable.map (fun url -> 
      use wc = new WebClient()
      url, wc.DownloadString(url) )
|> MiniObservable.subscribe (fun (url, body) ->
      printfn "%s (%d)" url body.Length)

// TASK #1: Implement `MiniObservable.mapAsync` so that we do not 
// need to block when downloading the pages using `WebClient`

// TASK #2: Implement `MiniObservable.head` with a type 
// `MiniObservable<'T> -> Async<'T>` which returns the first
// result that the observable produces, so that we can take
// just the first processed page!

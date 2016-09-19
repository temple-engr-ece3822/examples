#load "asyncseq.fs"
open Demos
open System
open System.IO
open System.Net
open System.Threading
open System.Text.RegularExpressions

// ------------------------------------------------------------------
// Helpers for crawling Wikipedia
// ------------------------------------------------------------------

// Override the default .NET limit for 2 connections
System.Net.ServicePointManager.DefaultConnectionLimit <- 20

// Regular expressions for extracting HTML elements :-)
let regexLink = Regex("\<a href=\"/wiki/[^\"]*\"")
let regexTitle = Regex("\<title\>[^\<]*\<")

let download (url:string) = 
  let wc = new WebClient()
  let html = wc.DownloadString(Uri(url))
  
  // Extract the contents of <title> from the page
  let title = regexTitle.Match(html).Value
  let title =
    if not (title.Contains("-")) then title
    else title.Substring(7, title.LastIndexOf('-')-8)

  // Extract all links to proper Wikipedia pages
  let allLinks = 
    [ for link in regexLink.Matches(html) do
        let atag = link.Captures.[0].Value
        if not (atag.Contains(":")) then
          let prefix = "http://en.wikipedia.org"
          yield prefix + atag.Split('"').[1] ]
  
  // Return the page title together with all the links
  title, allLinks

// ------------------------------------------------------------------
// Writing a crawler
// ------------------------------------------------------------------

let start = "http://en.wikipedia.org/wiki/Microsoft"
let rnd = Random() 

start 
|> AsyncSeq.unfoldAsync (fun url ->
    try 
      let title, links = download url
      Some(title, links.[rnd.Next(links.Length)]) 
    with _ -> None)
|> AsyncSeq.iter (fun title ->
    printfn "%s" title)
|> Async.Start

// TASK #1: Add asynchronous version of `unfold` named `unfoldAsync`
// so that we do not block when downloading the pages.


// TASK #2: Add `AsyncSeq.take` so that we can take the first 10
// pages and then the crawler stops.


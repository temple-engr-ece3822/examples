open System.Drawing

let colors = [ Color.Chocolate
               Color.Gray
               SystemColors.ButtonFace ]

let (|RGB|) (c :Color) = RGB(float c.R, float c.G, float c.B)

//NOTE: MSDN lists this as HSB, but the math on Wikipedia argues in favor of HSL
let (|HSL|) (c :Color) = HSL(c.GetHue         () |> float
                            ,c.GetSaturation  () |> float
                            ,c.GetBrightness  () |> float)

let (|MaxIsR|MaxIsB|MaxIsG|) (RGB (r,g,b)) =
  match r >= b, r >= g with
  | true  ,true   ->  MaxIsR r
  | true  ,false  ->  MaxIsG g
  | false ,true   ->  MaxIsB b 
  | false ,false  ->  if b >= g then MaxIsB b 
                                else MaxIsG g

//NOTE: Adapted from http://en.wikipedia.org/wiki/HSL_and_HSV
let (|HSV|) c =
  // Extract R,G, and B components and normalize to [0.0 ... 1.0]
  let (RGB (r,g,b)) = c
  let r,g,b = r / 255.0
            , g / 255.0
            , b / 255.0
  // Calculate "base" components
  let value  = max g (max b r)
  let chroma = value - (min g (min b r))
  // Determine hue and saturation
  let hue,saturation =
    match chroma with
    | 0.0     ->  // Color is achromatic (no hue or saturation)
                  0.0,0.0 
    | chroma  ->  // hue is based on which RGB component contributes the "value"
                  (60.0 * match c with
                          | MaxIsR _ -> (g - b) / chroma + (if g < b then 6.0 else 0.0) 
                          | MaxIsG _ -> (b - r) / chroma + 2.0
                          | MaxIsB _ -> (r - g) / chroma + 4.0)
                  // saturation is simply the ratio of chroma to value
                , (chroma / value)
  HSV (hue,saturation,value)

let printColorSpaces color =
  match color with
  | RGB (r,g,b) &
    HSV (h,s,v) &
    HSL (c,t,l) ->  // some real numbers really ought to be percentages
                    let t,l,s,v = (t * 100.0),(l * 100.0),(s * 100.0),(v * 100.0)
                    printfn "----------------------------"
                    printfn "%s" color.Name
                    printfn "----------------------------"
                    printfn "  RGB: %6.2f,%6.2f,%6.2f" r g b
                    printfn "  HSL: %6.2f,%6.2f,%6.2f" c t l
                    printfn "  HSV: %6.2f,%6.2f,%6.2f" h s v

for color in colors do printColorSpaces color

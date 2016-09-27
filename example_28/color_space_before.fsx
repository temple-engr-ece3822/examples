open System.Drawing

let colors = [ Color.Chocolate
               Color.Gray
               SystemColors.ButtonFace ]

let getHSV (c:Color) =
  // Extract R,G, and B components and normalize to [0.0 ... 1.0]
  let r,g,b = float c.R / 255.0
            , float c.G / 255.0
            , float c.B / 255.0
  // Calculate "base" components
  let value  = max g (max b r)
  let chroma = value - (min g (min b r))
  // Determine hue and saturation
  let hue,saturation =
    match chroma with
    | 0.0     ->  // Color is achromatic (no hue or saturation)
                  0.0,0.0 
    | chroma  ->  // hue is based on which RGB component contributes the "value"
                  (60.0 * match r >= b, r >= g with
                          | true  ,true   ->  (g - b) / chroma + (if g < b then 6.0 else 0.0)
                          | true  ,false  ->  (b - r) / chroma + 2.0
                          | false ,true   ->  (r - g) / chroma + 4.0
                          | false ,false  ->  if g >= b then (b - r) / chroma + 2.0
                                                        else (r - g) / chroma + 4.0)
                  // saturation is simply the ratio of chroma to value
                , (chroma / value)
  (hue,saturation,value)

let printColorSpaces (color:Color) =
  let r,g,b = float color.R
            , float color.G
            , float color.B
  let c,t,l = color.GetHue()
            // some real numbers really ought to be percentages
            , float (color.GetSaturation()) * 100.0
            , float (color.GetBrightness()) * 100.0
  let h,s,v = getHSV color
  let   s,v = (s * 100.0),(v * 100.0) // some real numbers really ought to be percentages

  printfn "----------------------------"
  printfn "%s" color.Name
  printfn "----------------------------"
  printfn "  RGB: %6.2f,%6.2f,%6.2f" r g b
  printfn "  HSL: %6.2f,%6.2f,%6.2f" c t l
  printfn "  HSV: %6.2f,%6.2f,%6.2f" h s v

for color in colors do printColorSpaces color

#Check out Python Tools for VS, super useful https://www.visualstudio.com/vs/python/
from lxml import html

# download & parse web page
doc = html.parse('http://apod.nasa.gov/apod/astropix.html').getroot()

# find <a href that ends with ".jpg" and 
# that has <img child that has src attribute that also ends with ".jpg"
for elem, attribute, link, _ in doc.iterlinks():
    if (attribute == 'href' and elem.tag == 'a' and link.endswith('.jpg') and
        len(elem) > 0 and elem[0].tag == 'img' and
        elem[0].get('src', '').endswith('.jpg')):
        print(link)
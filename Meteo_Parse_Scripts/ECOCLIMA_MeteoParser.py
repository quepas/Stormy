__author__ = 'Quepas'

from xml.dom import minidom

def process(text):
    parser = ECOCLIMA_MeteoParser()
    parser.data = {}
    parser.parseText(text.strip())
    return parser.data

class ECOCLIMA_MeteoParser():
    xml_dom = None
    data = {}

    def parseText(self, content):
        tbody = self.extractTBody(content)
        tbody = self.removeBadChars(tbody)
        self.parseTBody(tbody)

    def parseTBody(self, content):
        self.xml_dom = minidom.parseString(content)
        labels = self.xml_dom.getElementsByTagName("th")
        values = self.xml_dom.getElementsByTagName("td")
        size = len(values)

        for x, y in zip(labels, values):
            self.data.update({x.childNodes[0].nodeValue : y.childNodes[0].nodeValue})

    def extractTBody(self, html_site):
        html_site = '<tbody>' + html_site.split('<tbody class="aktualne">')[1]
        return html_site.split('</tbody>')[0] + '</tbody>'

    def removeBadChars(self, text):
        return text.replace("&nbsp;", " ").replace("&", "").replace("&deg;", "")
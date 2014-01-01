__author__ = 'Quepas'

from urllib import request
from xml.dom import minidom

def run(text, encoding="windows-1250"):
    parser = ECOCLIMA_MeteoParser()
    parser.parseFromURL(text, encoding)
    return parser.data

class ECOCLIMA_MeteoParser():
    xml_dom = None
    data = []

    def parseFromURL(self, url, encoding="windows-1250"):
        data = []
        sock = request.urlopen(url)
        content = str(sock.read(), encoding)
        tbody = self.extractTBody(content);
        tbody = self.removeBadChars(tbody)
        self.parseTBody(tbody)

    def parseTBody(self, content):
        self.xml_dom = minidom.parseString(content)
        labels = self.xml_dom.getElementsByTagName("th")
        values = self.xml_dom.getElementsByTagName("td")
        size = len(values)

        for x, y in zip(labels, values):
            self.data.append({x.childNodes[0].nodeValue : y.childNodes[0].nodeValue})

    def extractTBody(self, html_site):
        html_site = '<tbody>' + html_site.split('<tbody class="aktualne">')[1]
        return html_site.split('</tbody>')[0] + '</tr></tbody>'

    def removeBadChars(self, text):
        return text.replace("&nbsp;", " ").replace("&", "").replace("°", " ");

# Stacjameteo data reader
# python 2.7.6

import xml.etree.ElementTree as ETree

def process(text):
    parser = StacjameteoReader()
    parser.data={}
    parser.parseText(text.strip())
    print(parser.data)
    return parser.data

class StacjameteoReader:
    data = {}

    def parseText(self, text):
        text = self.prepareProperHtml(text)
        content = self.extractTable(text)
        content = self.addTBodyTags(content)
        root = ETree.fromstring(content)
        if root.tag == "tbody":
            self.parseData(root)

    def prepareProperHtml(self, text):
        return text.replace(" class=tr-0", "")\
                   .replace(" class=tr-1", "")\
                   .replace("&deg;", " deg")\
                   .replace("&sup2;", " sup2")\
                   .replace("<tr/>", "</tr>")

    def parseData(self, root):
        for child in root:
            if child.tag == "tr":
                row = self.parseSingleRow(child)
                if row != None:
                    self.data.update(row)

    def parseSingleRow(self, tr):
        if len(tr) == 2:
            dictItem = {
              tr[0][0].text.encode('unicode-escape') :
              tr[1][0].text.encode('unicode-escape')
            }
            return dictItem

    def extractTable(self, text):
        lowerText = text.lower()
        tableTagStart = lowerText.find("<table>")
        if tableTagStart > -1:
            tableTagEnd = lowerText.rfind("</table>")
            return text[tableTagStart+7 : tableTagEnd].strip()
        else:
            return text

    def addTBodyTags(self, text):
        return "<tbody>" + text + "</tbody>"
from html.parser import HTMLParser
from xml.parsers.expat import XMLParserType
from xml.parsers.expat import ParserCreate
import xml.etree.ElementTree as ETree
import re
from urllib import request

def run(text, encoding="UTF-8"):
    parser = MeteoBDataParser()
    parser.data=[]
    parser.parseFromHtml(text, encoding)
    #print(parser.data)
    return parser.data

class MeteoBDataParser(HTMLParser):
    data = []

    def parse(self, fileName, encoding="UTF-8"):
        text = open(fileName, encoding=encoding).read()
        content = self.extractTable(text)
        root = ETree.fromstring(content)
        if root.tag == "tbody":
            self.parseData(root)

    def parseFromHtml(self, url, encoding="UTF-8"):
        sock = request.urlopen(url)
        htmlSource = str(sock.read(), encoding)        
        self.parseText(htmlSource)
        sock.close()        

    def parseText(self, text):
        text = self.prepareProperHtml(text)
        content = self.extractTable(text)
        content = self.removeDiv(content)
        root = ETree.fromstring(self.addTBodyTags(content))
        if root.tag == "tbody":
            self.parseData(root)

    def prepareProperHtml(self, text):
        return text.replace(" class=tr-0", "")\
                   .replace(" class=tr-1", "")\
                   .replace("&deg;", "°")\
                   .replace("&sup2;", "²")\
                   .replace("<tr/>", "</tr>")\
                   .replace("\xb2", "");

    def parseData(self, root):
        for child in root:
            if child.tag == "tr":
                row = self.parseSingleRow(child)
                if row != None:
                    self.data.append(row)

    def parseSingleRow(self, tr):
        if len(tr) == 2:
            dictItem = {tr[0][0].text : tr[1][0].text}            
            #print(dictItem)
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

    def removeDiv(self, text):
        lowerText = text.lower()
        divEndTag = lowerText.find("</div>")
        if divEndTag > -1:
            divEndTag += 6
            return text[divEndTag:]
        else:
            return text

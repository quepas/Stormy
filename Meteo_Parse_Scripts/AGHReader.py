__author__ = 'quepas'
# AGH data reader
# python 2.7.6

from xml.dom import minidom

def process(text):
    parser = AGH_MeteoParser()
    parser.data = {}
    parser.parseXML(text)
    print(parser.data)
    return parser.data

class AGH_MeteoParser():
    xml_dom = None
    data = {}

    def parseXML(self, content):
        self.xml_dom = minidom.parseString(content)
        self.getDataAs("ta", "airTemperature")
        self.getDataAs("ua", "airHumidity")
        self.getDataAs("odew", "dewPoint")
        self.getDataAs("owindchill", "perceptibleTemperature")
        #self.getDataAs("oheatindex", "")
        self.getDataAs("sx", "windSpeedMax")
        self.getDataAs("sm", "windSpeedMedium")
        self.getDataAs("dm", "windDirectionAngle")
        self.getDataAs("pa", "atmosphericPressure")
        self.getDataAs("barosealevel", "barometricPressure")
        self.getDataAs("rc", "dropDaily")
        self.getDataAs("hc", "hail")
        self.getDataAs("ri", "dropIntensity")
        self.getDataAs("hi", "hailIntensity")
        self.getDataAs("tendency", "tendency")
        self.getDateTime()

    def getDataAs(self, tag_name, equivalent):
        self.data.update({
            equivalent.encode('unicode-escape') :
            self.xml_dom.getElementsByTagName(tag_name)[0].childNodes[0].nodeValue.encode('unicode-escape')
        })

    def getDateTime(self):
        datetime = self.xml_dom.getElementsByTagName("dane_aktualne")[0].attributes["data"].value
        self.data.update({"datetime" : datetime.encode('unicode-escape')})

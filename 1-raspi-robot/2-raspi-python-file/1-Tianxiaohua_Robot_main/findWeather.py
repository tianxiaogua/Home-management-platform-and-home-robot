import urllib.request
import re

def change_value(value = ''):
    number =re.findall('(\d+)',value)
    if value[:1].find('-'):
        #'-' not in value[:2]:
        result = '零下'+number[0]+","
    else:
        result = '零上' + number[0]+","
    if value[1:].find('-'):
        result = result+ '至' + '零下'+ number[1]+","
    else:
        result = result + '至' + '零上' + number[1]
    return result+'摄氏度'



def return_weather():
    url = "http://www.weather.com.cn/weather1d/101070702.shtml#input"
    get=urllib.request.urlopen(url).read()

    wether = {"现在天气":[],
              "12h小时内":[],
              "24小时内":[]
              }
    html = str(get, encoding="utf-8")#把网页的bite数据类型转换成字符串

    a = html.find("hidden_title")
    b = html[a:].find(">")+a
    value = html[a:b].split(" ")
    wether["现在天气"].append(value[4])
    wether["现在天气"].append(change_value(value[6]))
    #print("现在天气：",wether["现在天气"])

    a = html[b:].find('wea" title=')+b+12
    b = html[a:].find('"')+a
    wether["12h小时内"].append(html[a:b])

    a = html[b:].find('<span class="" title="')+b+len('<span class="" title="')
    b = html[a:].find("</span>")+a
    c = html[a:b].find('">')+a
    wether["12h小时内"].append(html[a:c])
    wether["12h小时内"].append(html[c+2:b])
    #print("12小时内：",wether["12h小时内"])

    a = html[b:].find('"wea" title="')+b+len('"wea" title="')
    b = html[a:].find('">')+a
    wether["24小时内"].append(html[a:b])

    #print(html[a:])
    a = html[b:].find('<span>')+b+len('<span>')
    b = html[a:].find("</span>")+a
    wether["24小时内"].append(html[a:b])

    a = html[b:].find('class="" title="')+b+len('class="" title="')
    b = html[a:].find("</span>")+a
    c = html[a:b].find('">')+a
    wether["24小时内"].append(html[a:c])
    wether["24小时内"].append(html[c+2:b])
    #print("24小时内",wether["24小时内"])
    return wether

if __name__ == '__main__':
    a = return_weather()
    print(a)

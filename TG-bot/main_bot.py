import telebot
from auth_data import token
import re
import requests
from bs4 import BeautifulSoup
import time 

# bot = telebot.TeleBot(token)

# @bot.message_handler(commands=['start'])
# def start(message):
#   sent = bot.send_message(message.chat.id, 'Please describe your problem.')
#   bot.register_next_step_handler(sent, hello)

# def hello(message):
#     print(message.chat.id , message.text)
#     bot.send_message(message.chat.id , 'Your chat id is : %s. Thank you!' % message.chat.id )
#     bot.send_message(message.chat.id , " {chat_id} |  {message_text}".format(chat_id=message.chat.id, message_text=message.text))

# bot.polling()

# URL = 'https://minfin.com.ua/ua/currency/'

# def get_data():
#     r = requests.get(URL)
#     soup = BeautifulSoup(r.text, features="html.parser")
#     count = soup.find_all('td', class_='mfm-text-nowrap', attrs={'data-title': 'Чорний ринок'})
#     count_lists = []

#     for list1 in count:
#         count_lists.append(str(list1.get_text().replace('\n','')))

#     return count_lists
# get_data()
# URL = input('Введіть посилання на сайт = ')
# URL = 'https://espreso.tv/news/'
HEADER = {
    'accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9',
    'user-agent': 'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.112 Safari/537.36'
}

html = ''
string = ''
lists = []
soup = []
parent = []
wait_link = []

def get_html(url):
    r = requests.get(url,headers=HEADER)
    return r

def get_content(html,string):

    def not_lacie(text):
        return text and re.compile(string,flags=re.IGNORECASE).search(text) 

    global soup
    soup = BeautifulSoup(html, features="html.parser")

    for s in soup.select('script'):
        s.extract()
    for s in soup.select('style'):
        s.extract()
    global lists
    lists = soup.find_all(text=not_lacie)
    
    for s in lists:
        if len(s.split()) == 1:
            lists.remove(s)
        else:
            pass

    return lists

def find_parent(num):

    global parent
    parent = soup.find(text=num).find_previous()
    # parent = parent.get_text()

    return parent

def parcer():
    get_content(html.text,string)

def telegram_bot(token):

    bot = telebot.TeleBot(token)
    keyboard1 = telebot.types.ReplyKeyboardMarkup(True,True)
    keyboard1.row('/parce')

    keyboard2 = telebot.types.ReplyKeyboardMarkup(True,True)
    keyboard2.row('yes','no')

    @bot.message_handler(commands=['start'])
    def start_message(message):
        bot.send_message(message.chat.id, 'Привіт, ти написав мені /start.', reply_markup=keyboard1)

    @bot.message_handler(commands=['parce'])
    def help_message(message):
        sent = bot.send_message(message.chat.id, "<strong> Введіть посилання на сайт: </strong>", parse_mode='HTML')
        bot.register_next_step_handler(sent, check_url)

    @bot.message_handler(commands=['stop'])
    def stop_search(message):
        answer = bot.send_message(message.chat.id, 'Продовжити пошук на даному сайті',reply_markup=keyboard2)
        bot.register_next_step_handler(answer, restart)

    @bot.message_handler(commands=['search'])
    def search(message):
        if message.text == '/search':
            bot.send_message(message.chat.id, 'Ви забули число')
        else:
            num = message.text.replace('/search ', '')
            find_string = lists[int(num)]
            find_parent(find_string)
            print(parent)
            search_answers(message)

    def search_answers(message):
        bot.send_message(message.chat.id, parent)
        answer_parent = bot.send_message(message.chat.id, 'Продовжити пошук ?',reply_markup=keyboard2)
        bot.register_next_step_handler(answer_parent, advance_search)

    def advance_search(message):
        if message.text.lower() == 'yes':
            print(parent)
            find_parent(parent.get_text())
            search_answers(message)
        elif message.text.lower() == 'no':
            stop_search()

    def check_url(message):
        # wait_link = []
        global wait_link
        wait_link = message
        print(wait_link)
        try:
            reston = requests.get(message.text)
            if reston.status_code == 200:
                global html
                html = get_html(message.text)
                bot.send_message(message.chat.id , 'Your URL is : %s.' % message.text )
                string = bot.send_message(message.chat.id , '<strong>Введіть те що потрібно знайти (достатньо декількох слів):</strong>', parse_mode='HTML')
                bot.register_next_step_handler(string, get_text)

        except requests.exceptions.MissingSchema:
            bot.reply_to(message, 'Please, send me a valid link.\nhttp:// might be necessary.')
        except requests.exceptions.InvalidSchema:
            bot.reply_to(message, 'Please, send me a valid link.\nhttp:// might be necessary.')
        except requests.exceptions.InvalidURL:
            bot.reply_to(message, 'Invalid URL')
    
    def get_text(message):
        if message.text != '':
            global string
            string = message.text.lower()
            seconds = 0
            seconds = time.time()
            parcer()
            bot.send_message(message.chat.id,'Time : %s' % str ((time.time() - seconds )*1000))
            if len(lists) == 1: 
                bot.send_message(message.chat.id, '<b>Було знайдено %s результат </b>' % len(lists), parse_mode='HTML')
            elif len(lists) > 1 :
                bot.send_message(message.chat.id, '<b>Було знайдено %s результатів </b>' % len(lists), parse_mode='HTML')
            elif not lists:
              bot.send_message(message.chat.id, '<b>Результатів не знайдено </b>', parse_mode='HTML')
            # if len(lists) > 25:
            #     with open("test.txt", 'w') as f:
            #         for i in range(len(lists)):
            #             f.write('Результат %s: %s \n' % (i,lists[i]))
            #         f.close()
            #     bot.send_document(message.chat.id,f)
            #     # time.sleep(0.01)
            # else:
            for i in range(len(lists)):
                bot.send_message(message.chat.id, 'Результат %s: %s' % (i,lists[i]))
                time.sleep(0.01)
            
            bot.send_message(message.chat.id, 'Якщо в представлених результатах немає потрібної вам інформації напишість "/search <число_результат> ". '
            + 'Число_результат - це число результату який найближче знаходиться до загаданого вами тексту. '
            + 'Якщо вас все вмаштовує то напишіть /stop')

        elif message.text == '':
            bot.reply_to(message, 'Message is empty') 
        else:
            bot.reply_to(message, 'Something wrong ')

    def restart(message):
        if message.text.lower() == 'yes':
            check_url(wait_link)
        elif message.text.lower() == 'no':
            bot.send_message(message.chat.id, 'By', reply_markup=keyboard1)
        
    @bot.message_handler(content_types=['dice'])
    def sticker_id(message):
        print(message)

    bot.polling()

if __name__ == '__main__':
    telegram_bot(token)
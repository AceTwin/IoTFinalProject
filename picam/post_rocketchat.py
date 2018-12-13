from pprint import pprint
from rocketchat_API.rocketchat import RocketChat

rocket = RocketChat('StrangerCalling', 'user password', server_url='http://192.168.86.120:3000')
pprint(rocket.me().json())
pprint(rocket.channels_list().json())
pprint(rocket.chat_post_message('good news everyone!', channel='GENERAL').json())
pprint(rocket.channels_history('GENERAL', count=5).json())

#Code from https://pypi.org/project/rocketchat-API/

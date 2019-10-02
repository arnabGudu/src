import wiringpi as wp
import time
import rospy

INA1 = 20
INA2 = 16
INB1 = 5
INB2 = 6
ENA = 12
ENB = 13

from std_msgs.msg import Int16 

wp.wiringPiSetupGpio()

wp.pinMode(INA1, 1)
wp.pinMode(INA2, 1)

wp.pinMode(INB1, 1)
wp.pinMode(INB2, 1)

wp.pinMode(ENA, 2)
wp.pinMode(ENB, 2)

wp.digitalWrite(INA1, HIGH)
wp.digitalWrite(INA2, LOW)
wp.digitalWrite(INB1, HIGH)
wp.digitalWrite(INB2, LOW)

def callback(data):
	wp.pwmWrite(ENA, 120 + data);
	wp.pwmWrite(ENB, 120 - data);
	
def listener():
	rospy.init_node('motor', anonymous = True)
	rospy.Subscriber('speed', Int16, callback)
	rospy.spin()

if __name__ == '__main__':
	listener()

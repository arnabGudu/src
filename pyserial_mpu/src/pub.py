import serial
import time
import rospy

from std_msgs.msg import String
from geometry_msgs.msg import Vector3

ser = serial.Serial('/dev/ttyACM0', 115200)
    
def talker():
	pub = rospy.Publisher('mpu', String, queue_size=1000);
    	rospy.init_node('pyserial_mpu', anonymous=True)

	while not rospy.is_shutdown():
		_read = ser.readline()
		_list = _read.split(',')
		_list = 
		#pub.publish(msg)

if __name__ == '__main__':
	talker()

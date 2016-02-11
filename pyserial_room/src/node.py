import serial
import time
import rospy

from std_msgs.msg import Int32

ser = serial.Serial('/dev/ttyUSB0', 115200)

print("working")

def callback(data):
    rospy.loginfo(rospy.get_caller_id() + "I heard %d", data.data)
    ser.write(str(data.data))
    
def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("camera", Int32, callback)

    rospy.spin()

if __name__ == '__main__':
    listener()

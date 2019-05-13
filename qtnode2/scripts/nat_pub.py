#!/usr/bin/env python
# license removed for brevity
import rospy
from std_msgs.msg import Int32

def talker():
    pub = rospy.Publisher('numbers', Int32, queue_size=10)
    rospy.init_node('bcast', anonymous=True)
    rate = rospy.Rate(2)
    i=0
    while not rospy.is_shutdown():
        i+=1
        rospy.loginfo(i)
        pub.publish(i)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass

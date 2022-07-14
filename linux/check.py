#!/usr/bin/env python3
# encoding:utf-8
# 2020/01/25 aiden
import time
import numpy as np
from math import sqrt
import inverse_kinematics
import json

#机械臂根据逆运动学算出的角度进行移动
ik = inverse_kinematics.IK()

class ArmIK:
    servo3Range = (0, 1000, 0, 240.0) #脉宽， 角度
    servo4Range = (0, 1000, 0, 240.0)
    servo5Range = (0, 1000, 0, 240.0)
    servo6Range = (0, 1000, 0, 240.0)

    def __init__(self):
        self.setServoRange()

    def setServoRange(self, servo3_Range=servo3Range, servo4_Range=servo4Range, servo5_Range=servo5Range, servo6_Range=servo6Range):
        # 适配不同的舵机
        self.servo3Range = servo3_Range
        self.servo4Range = servo4_Range
        self.servo5Range = servo5_Range
        self.servo6Range = servo6_Range
        self.servo3Param = (self.servo3Range[1] - self.servo3Range[0]) / (self.servo3Range[3] - self.servo3Range[2])
        self.servo4Param = (self.servo4Range[1] - self.servo4Range[0]) / (self.servo4Range[3] - self.servo4Range[2])
        self.servo5Param = (self.servo5Range[1] - self.servo5Range[0]) / (self.servo5Range[3] - self.servo5Range[2])
        self.servo6Param = (self.servo6Range[1] - self.servo6Range[0]) / (self.servo6Range[3] - self.servo6Range[2])

    def transformAngelAdaptArm(self, theta3, theta4, theta5, theta6):
        #将逆运动学算出的角度转换为舵机对应的脉宽值
        servo3 = int(round(theta3 * self.servo3Param + (self.servo3Range[1] + self.servo3Range[0])/2))

        servo4 = int(round(-theta4 * self.servo4Param + (self.servo4Range[1] + self.servo4Range[0])/2))

        servo5 = int(round((self.servo5Range[1] + self.servo5Range[0])/2 + theta5 * self.servo5Param))
        
        servo6 = int(round(((self.servo6Range[3] - self.servo6Range[2])/2 + theta6)) * self.servo6Param)

        return {"servo3": servo3, "servo4": servo4, "servo5": servo5, "servo6": servo6}
    
    def setPitchRanges(self, coordinate_data, alpha, alpha1, alpha2, d = 0.01):
        #给定坐标coordinate_data和俯仰角alpha,以及俯仰角范围的范围alpha1, alpha2，自动寻找最接近给定俯仰角的解
        #如果无解返回False,否则返回舵机角度、俯仰角
        #坐标单位m， 以元组形式传入，例如(0, 0.5, 0.1)
        #alpha为给定俯仰角, 单位度
        #alpha1和alpha2为俯仰角的取值范围

        x, y, z = coordinate_data
        a_range = abs(int(abs(alpha1 - alpha2)/d)) + 1
        for i in range(a_range):
            if i % 2:
                alpha_ = alpha + (i + 1)/2*d
            else:                
                alpha_ = alpha - i/2*d
                if alpha_ < alpha1:
                    alpha_ = alpha2 - i/2*d
##            print(alpha_)
            result = ik.getRotationAngle((x, y, z), alpha_)
            if result:
                theta3, theta4, theta5, theta6 = result['theta3'], result['theta4'], result['theta5'], result['theta6']
                servos = self.transformAngelAdaptArm(theta3, theta4, theta5, theta6)
                return result, servos, alpha_
        
        return False


import rospy
import armpi_fpv.bus_servo_control as bus_servo_control
from hiwonder_servo_msgs.msg import MultiRawIdPosDur
    
    # 初始化节点
rospy.init_node('ik_test', log_level=rospy.DEBUG)
    
    # 舵机发布
joints_pub = rospy.Publisher('/servo_controllers/port_id_1/multi_id_pos_dur', MultiRawIdPosDur, queue_size=1)
rospy.sleep(0.2)

def armlist_generate():
    numlist = ['a','b','c','d','e','f','g','h']
    armlist = []
    for i in numlist:
        for j in range(8):
            k = i+str(j+1)
            armlist.append(k)
    return armlist # a1--h8

close = 165
loose = 90

def common_draw(x,y,z,grab):
    AK = ArmIK()  
    target = AK.setPitchRanges((x/1000,y/1000,z/1000), -180, -180, 0)
    if target:
        servo_data = target[1]
        bus_servo_control.set_servos(joints_pub, 500, ((1, grab), (2, 500), (3, servo_data['servo3']), (4, servo_data['servo4']), (5, servo_data['servo5']), (6, servo_data['servo6'])))
        rospy.sleep(1)
def test(x,y,z):
    common_draw(x,y,z,loose)
    
def move1(x, y, z):
    common_draw(x,y,z+30,loose)  # qi zi shang fang 
    common_draw(x,y,z,loose)        # qi zi wei zhi
    common_draw(x,y,z,close)            # zhua zi
    common_draw(x,y,z+30,close)      # tai zi
def move2(x, y, z):
    common_draw(x,y,z+30,close)      # yi dong zi
    common_draw(x,y,z,close)            # fang xia zi
    common_draw(x,y,z,loose)        # song kai zhi
    common_draw(x,y,z+30,loose)  # shang yi 
    
if __name__ == "__main__":
    armlist = armlist_generate()
    with open('armdict.txt','r') as f:
        armditext = f.readline()
        armdict = eval(armditext)
    while True:
        function_chosen = input('输入1测试all, input 2 test like a1a2, 输入5测试一列, 退出按q:')
        if function_chosen == '1':
            for i in armlist:
                v=armdict[i]
                x,y,z = v[0],v[1],v[2]
                print('{0:s}坐标：'.format(i),x,y,z)
                test(x,y,z)
                rospy.sleep(1)
        elif function_chosen == '5':
            col_test = input('请输入列，比如a 则自动测试a1-a8：')
            num = 0
            for num in range(8):
                i = col_test+str(num+1)
                v = armdict[i]
                x,y,z = v[0],v[1],v[2]
                print('{0:s}坐标：'.format(i),x,y,z)
                test(x,y,z)
                rospy.sleep(1)
        elif function_chosen == '2':
            while True:
                str = input('input like a1,a2   input q quit   input y change： ')
                if str == 'q':
                    break
                elif str == 'y':
                    cord_chosen = input('输入坐标代号，比如a1:')
                    u = armdict[cord_chosen]
                    x,y,z = u[0],u[1],u[2]
                    while True:
                        print('现在的坐标:',x,y,z)
                        try:
                            x,y,z = input('请输入x,y,z空格隔开:').split()
                            x = eval(x)
                            y = eval(y)
                            z = eval(z)
                            armdict[cord_chosen]=(x,y,z)
                            print('更新后的值：',armdict[cord_chosen])
                            test(x,y,z)
                            rospy.sleep(1)
                        except:
                            print('输错了')
                            break
                else:
                    # v1
                    str1 = str[0:2]
                    v1 = armdict[str1]
                    x1,y1,z1 = v1[0],v1[1],v1[2]
                    print('{0:s}坐标：'.format(str1),x1,y1,z1)
                    move1(x1,y1,z1)
                    rospy.sleep(1)
                    # V2
                    str2 = str[2:4]
                    v2= armdict[str2]
                    x2,y2,z2 = v2[0],v2[1],v2[2]
                    print('{0:s}坐标：'.format(str2),x2,y2,z2)
                    move2(x2,y2,z2)
                    rospy.sleep(1)
        elif function_chosen == 'q':
            break
        else:
            print('输入错误')

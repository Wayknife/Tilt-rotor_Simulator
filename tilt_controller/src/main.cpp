#include <iostream>
#include <fstream>
#include <csignal>
#include <sys/time.h>
#include "ros/ros.h"
#include "tilt_srv/Imu_srv.h"
#include "tilt_srv/Vector3Stamped_srv.h"
#include "tilt_srv/NavSatFix_srv.h"
#include "tilt_srv/Range_srv.h"
#include "tilt_srv/Sensor_srv.h"
#include "tilt_srv/Ref.h"
#include "tilt_srv/States.h"
#include "std_msgs/String.h"
#include<Eigen/Eigen>
#include <boost/thread.hpp>

using namespace Eigen;


// variáveis globais
struct sigaction sa;
struct itimerval timer;

// clientes
ros::ServiceClient imu;
ros::ServiceClient gps_fix;
ros::ServiceClient gps_velocity;
ros::ServiceClient magnetic;
ros::ServiceClient sonar;
ros::ServiceClient servo_esq_srv;
ros::ServiceClient servo_dir_srv;
ros::ServiceClient thrust_esq_srv;
ros::ServiceClient thrust_dir_srv;

// estruturas de comunicação
tilt_srv::Imu_srv values_imu;
tilt_srv::NavSatFix_srv values_fix_gps;
tilt_srv::Vector3Stamped_srv values_velocity_gps;
tilt_srv::Vector3Stamped_srv values_magnetic;
tilt_srv::Range_srv values_sonar;
tilt_srv::Sensor_srv values_servo_esq;
tilt_srv::Sensor_srv values_servo_dir;
tilt_srv::Sensor_srv values_thrust_esq;
tilt_srv::Sensor_srv values_thrust_dir;

// mensagens
tilt_srv::Ref thrustdir;
tilt_srv::Ref thrustesq;
tilt_srv::Ref servodir;
tilt_srv::Ref servoesq;


// publicadores
ros::Publisher thrustdir_pub;
ros::Publisher thrustesq_pub;
ros::Publisher servodir_pub;
ros::Publisher servoesq_pub;
ros::Publisher Step_pub;

//subscribers
ros::Subscriber X_sub;

tilt_srv::States msg2;

VectorXd Xref(16);
VectorXd Input(4);
MatrixXd K(4,16);

std::fstream out;
std::fstream in;

void X_Callback(tilt_srv::States msg)
{
	// lendo sensores	
	VectorXd X(16);	
	out<<   msg.header.seq << ","<<
		msg.x << ","<<
		msg.y <<","<< 
		msg.z <<","<< 
		msg.r<< ","<<
		msg.p<< ","<<
		msg.yaw<< ","<<
		msg.aR<< ","<<
		msg.aL<< ","<<
		msg.vx<< ","<<
		msg.vy<< ","<<
		msg.vz<< ","<<
		msg.dr<< ","<<
		msg.dp<< ","<<
		msg.dyaw<< ","<<
		msg.daR<< ","<<
		msg.daL << std::endl;
	
	X << msg.x,msg.y,msg.z,msg.r,msg.p,msg.yaw,msg.aR,msg.aL,msg.vx,msg.vy,msg.vz,msg.dr,msg.dp,msg.dyaw,msg.daR,msg.daL;

	// Controlador Linear
	Input = -K*(X-Xref); 

 	thrustdir.ref = Input(0); //+ 7.742354911319124;
	thrustesq.ref = Input(1); //+ 7.452125992789735;
	servodir.ref = Input(2) + 0;
	servoesq.ref = Input(3) + 0;

	/*thrustdir.ref = 0;
	thrustesq.ref = 0;
	servodir.ref = 0;
	servoesq.ref = 0;*/	
	
	in <<   msg2.header.seq << ","<<
		thrustdir.ref << ","<<
		thrustesq.ref << ","<<
		servodir.ref << ","<<
		servoesq.ref << std::endl;

	// Atuadores
	thrustdir_pub.publish(thrustdir);
	thrustesq_pub.publish(thrustesq);
	servodir_pub.publish(servodir);
	servoesq_pub.publish(servoesq);

	// Comando para passo de simulação
	std_msgs::String msgpub;
    	std::stringstream ss;
    	ss << "GO" ;
    	msgpub.data = ss.str();
	Step_pub.publish(msgpub);
}

int main (int argc, char **argv)
{
	out.open ("/home/macro/catkin_ws/src/Tilt-rotor_Simulator/tilt_controller/output.txt", std::fstream::out | std::fstream::app);
	in.open ("/home/macro/catkin_ws/src/Tilt-rotor_Simulator/tilt_controller/input.txt", std::fstream::out | std::fstream::app);
  	
	Xref << 0,0,0,-0.000155986,-0.0173469,0,0.017397000000000,0.017165300000000,0,0,0,0,0,0,0,0;

/*Primeira linha*/
K << 0.001061208112855,   3.095220631112246,   3.560259191233044, -10.729863657434219,   0.015774842413909,  0.806485610663944 ,  0.078999674488821,  -0.078665607273218,   0.001761121390644,   2.692819022376055,  2.775554316556502,  -1.776711096494242 ,  0.009189511990119 ,  0.167417958317943 ,  0.000387922564067 , -0.000386956237561 , 

/*Segunda linha*/
0.000382677066972 , -3.084474523110329 ,  3.572734418588530 , 10.697582615095177 ,  0.014038859249674 , -0.803603125457992,  -0.078433125970031 ,  0.078662281974900 ,  0.001262669612252 , -2.683823938441937 , 2.785587425683395  , 1.774026438197887  , 0.008976607359441 , -0.166818523401276 , -0.000385315158708 , 0.000386751979629,

/*Terceira linha*/
0.025448410051929 , -0.002296600329527 , -0.000005342627583 ,  0.009643830908894 ,  0.096259656009916  , 0.080682747951477  , 0.292143235857104  , 0.004770179053811 ,  0.023059684955404 , -0.002191018705070 , 0.000002062445698 ,  0.002012182664739 ,  0.017664775520566 ,  0.023708420892481 ,  0.007670512402321 ,  0.000131179127006, 

/*Quarta linha*/
0.025502593704066 ,  0.002285851917801 , -0.000005147036032 , -0.009594110894782 ,  0.096443902991815 , -0.080478046615840 ,  0.004819275689272 ,  0.292403136560160 ,  0.023108709249397  ,  0.002180242417387 ,  0.000002014740003 , -0.002001060387771 ,  0.017684500426855 , -0.023648191787312 ,  0.000131725190320  ,  0.007672528576484;

	// declarando nó
	ros::init(argc, argv, "tilt_controller");
	ros::NodeHandle n;
	
	// declarando publicadores
	thrustdir_pub = n.advertise<tilt_srv::Ref>("/Tilt/RefThrustdir", 10);
	thrustesq_pub = n.advertise<tilt_srv::Ref>("/Tilt/RefThrustesq", 10);
	servodir_pub = n.advertise<tilt_srv::Ref>("/Tilt/RefaL", 10);
	servoesq_pub = n.advertise<tilt_srv::Ref>("/Tilt/RefaR", 10);
	Step_pub = n.advertise<std_msgs::String>("Step", 1);	

	// declarando subscribers
	ros::Subscriber sub = n.subscribe("/Tilt/Teste", 1, X_Callback);

	ros::spin();
	
	out.close();
	in.close();	
	return 0;
}

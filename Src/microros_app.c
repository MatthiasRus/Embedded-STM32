/*
 * microros_app.c
 *
 *  Created on: May 20, 2026
 *      Author: mat-ros
 */

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <sensor_msgs/msg/joint_state.h>

#include "microros_transport.h"
#include "FreeRTOS.h"
#include "task.h"


void microros_task(void* args){
	rcl_publisher_t pub = rcl_get_zero_initialized_publisher();
	rcl_context_t context;
	rclc_executor_t exec;

	rcl_allocator_t allocator = rcl_get_default_allocator();

	rmw_uros_set_custom_transport(
			true,
			(void*)args,
			transport_open,
			transport_close,
			transport_write,
			transport_read
			);

	rcl_init_options_t init_options = rcl_get_zero_initialized_init_options();
	rcl_init_options_init(&init_options, allocator);
	rcl_init(0, NULL, &init_options, &context);

	rcl_node_t node = rcl_get_zero_initialized_node();
	rcl_node_options_t node_options = rcl_node_get_default_options();

	rcl_node_init(&node, "arm_mros_node", "/arm", &context, &node_options);

	const rosidl_message_type_support_t js_type_support = ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs,msg,JointState);

	rclc_publisher_init_default(&pub, &node, &js_type_support, "/joint_states");

	rclc_executor_init(&exec, &context, 1, &allocator);

	sensor_msgs__msg__JointState msg = {0};

	while(1){

		rcl_publish(&pub, &msg, NULL);
		rclc_executor_spin_some(&exec, RCL_MS_TO_NS(10));
		vTaskDelay(pdMS_TO_TICKS(10));

	}


}

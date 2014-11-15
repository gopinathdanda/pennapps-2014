#include <Python.h>
#include <stdio.h>
#include <bcm2835.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// PIN1 is right side
#define PIN1 RPI_GPIO_P1_11
// PIN2 is left side
#define PIN2 RPI_GPIO_P1_12

// Signal time
static const int DELAY = 5000;
static const int DELAY_BACK = 537;
static const int DELAY_FORWARD = 4436;
static const int DELAY_BETWEEN = 3899;

/*  wrapped cosine function */
static PyObject* turn_left(PyObject* self, PyObject* args)
{
	int i=0;
	while(i<100){
		// To turn left, PIN1 ->forward, PIN2 ->forward
		bcm2835_gpio_write(PIN2,HIGH);
		bcm2835_gpio_write(PIN1,HIGH);

		bcm2835_delayMicroseconds(DELAY_BACK);

		bcm2835_gpio_write(PIN2,LOW);
		bcm2835_gpio_write(PIN1,LOW);
	
		bcm2835_delayMicroseconds(DELAY_FORWARD);

		printf("turning left\n");
		i++;
	}
	return Py_BuildValue("d", 0);
}
static PyObject* turn_right(PyObject* self, PyObject* args)
{             
	int i=0;
	while(i<100){
		//to turn right PIN1->backwards, PIN2-> forward 
                bcm2835_gpio_write(PIN1,HIGH);
                bcm2835_gpio_write(PIN2,HIGH);
                bcm2835_delayMicroseconds(DELAY_FORWARD);
                bcm2835_gpio_write(PIN1,LOW);
                bcm2835_gpio_write(PIN2,LOW);
                bcm2835_delayMicroseconds(DELAY_BACK);
                printf("turning right\n");
		i++;
	}
                return Py_BuildValue("d", 0);
}
static PyObject* move_backward(PyObject* self, PyObject* args)
{         
	int i=0;
	while(i<100){     
                bcm2835_gpio_write(PIN2,HIGH);
                bcm2835_gpio_write(PIN1,HIGH);
                bcm2835_delayMicroseconds(DELAY_BACK);
                bcm2835_gpio_write(PIN2,LOW);
                bcm2835_delayMicroseconds(DELAY_BETWEEN);
                bcm2835_gpio_write(PIN1,LOW);
                bcm2835_delayMicroseconds(DELAY_FORWARD);
                printf("moving backward\n");
		i++;
	}
                return Py_BuildValue("d", 0);
}
static PyObject* move_forward(PyObject* self, PyObject* args)
{  
	int i=0;
	while(i<100){
                bcm2835_gpio_write(PIN2,HIGH);
                bcm2835_gpio_write(PIN1,HIGH);
                bcm2835_delayMicroseconds(DELAY_BACK);
                bcm2835_gpio_write(PIN1,LOW);
                bcm2835_delayMicroseconds(DELAY_BETWEEN);
                bcm2835_gpio_write(PIN2,LOW);
                bcm2835_delayMicroseconds(DELAY_BACK);
                printf("moving forward\n");
		i++;
	}
                return Py_BuildValue("d", 0);

}

/*  define functions in module */
static PyMethodDef methods[] =
{
	{"turn_left", turn_left, METH_VARARGS, "turn left"},
        {"turn_right", turn_right, METH_VARARGS, "turn right"},
        {"move_backward", move_backward, METH_VARARGS, "move backward"},
        {"move_forward", move_forward, METH_VARARGS, "move forward"},
        {NULL, NULL, 0, NULL}


};

static struct PyModuleDef pennapprobotmodule = {
	PyModuleDef_HEAD_INIT,
	"pennapprobot",   /* name of module */
	NULL, /* module documentation, may be NULL */
	-1,       /* size of per-interpreter state of the module,
				                 or -1 if the module keeps state in global variables. */
	methods
};

/* module initialization */
PyMODINIT_FUNC

PyInit_pennapprobot(void)
{
	PyObject *m;

	m = PyModule_Create(&pennapprobotmodule);

	bcm2835_init();
	bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);

	if (m == NULL)
		return NULL;

        return m;
}

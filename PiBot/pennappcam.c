#include <Python.h>
#include <stdio.h>
#include <bcm2835.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// PIN1 is lower
#define PIN1 RPI_GPIO_P1_16
// PIN2 is upper
#define PIN2 RPI_GPIO_P1_15

// Signal time
static const int DELAY = 5000;
static const int DELAY_BACK = 537;
static const int DELAY_FORWARD = 4436;
static const int DELAY_BETWEEN = 3899;

static PyObject* lower_r(PyObject* self, PyObject* args)
{
        int i=0;
        while(i<1){
                bcm2835_gpio_write(PIN1,HIGH);
                bcm2835_delayMicroseconds(DELAY_BACK);
                bcm2835_gpio_write(PIN1,LOW);
                bcm2835_delayMicroseconds(DELAY_BETWEEN);
                bcm2835_delayMicroseconds(DELAY_FORWARD);
                printf("bottom");
                i++;
        }
        return Py_BuildValue("d", 0);
}

static PyObject* lower_l(PyObject* self, PyObject* args)
{
        int i=0;
        while(i<1){
                bcm2835_gpio_write(PIN1,HIGH);
                bcm2835_delayMicroseconds(DELAY_BACK);
                bcm2835_delayMicroseconds(DELAY_BETWEEN);
                bcm2835_gpio_write(PIN1,LOW);
                bcm2835_delayMicroseconds(DELAY_BACK);
                printf("one step up");
                i++;
        }
        return Py_BuildValue("d", 0);
}


static PyObject* upper_b(PyObject* self, PyObject* args)
{
        int i=0;
        while(i<3){
                bcm2835_gpio_write(PIN2,HIGH);
                bcm2835_delayMicroseconds(DELAY_BACK);
		bcm2835_gpio_write(PIN2,LOW);
		bcm2835_delayMicroseconds(DELAY_BETWEEN);
		bcm2835_delayMicroseconds(DELAY_FORWARD);
                printf("one step up");
                i++;
        }
        return Py_BuildValue("d", 0);
}

static PyObject* upper_f(PyObject* self, PyObject* args)
{
        int i=0;
        while(i<1){
                bcm2835_gpio_write(PIN2,HIGH);
                bcm2835_delayMicroseconds(DELAY_BACK);
		bcm2835_delayMicroseconds(DELAY_BETWEEN);
                bcm2835_gpio_write(PIN2,LOW);
                bcm2835_delayMicroseconds(DELAY_BACK);
                printf("one step up");
                i++;
        }
        return Py_BuildValue("d", 0);
}


/*  define functions in module */
static PyMethodDef methods[] =
{
        {"upper_f", upper_f, METH_VARARGS, "move upper forward"},
        {"upper_b", upper_b, METH_VARARGS, "move upper backward"},
        {"lower_l", lower_l, METH_VARARGS, "move lower left"},
        {"lower_r", lower_r, METH_VARARGS, "move lower right"},
        {NULL, NULL, 0, NULL}
};

static struct PyModuleDef pennappcammodule = {
        PyModuleDef_HEAD_INIT,
        "pennappcam",   /* name of module */
        NULL, /* module documentation, may be NULL */
        -1,       /* size of per-interpreter state of the module,
                                                 or -1 if the module keeps stat*/
        methods
};

/* module initialization */
PyMODINIT_FUNC

PyInit_pennappcam(void)
{
        PyObject *m;

        m = PyModule_Create(&pennappcammodule);

        bcm2835_init();
        bcm2835_gpio_fsel(PIN1, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_fsel(PIN2, BCM2835_GPIO_FSEL_OUTP);

        if (m == NULL)
                return NULL;

        return m;
}

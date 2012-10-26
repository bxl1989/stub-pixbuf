// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This is the simplest possible C Pepper plugin that does nothing. If you're
// using C++, you will want to look at stub.cc which uses the more convenient
// C++ wrappers.

//#include "stub-pixbuf.h"


//-------------------------------------------------------------------

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>
#include <pthread.h>

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppp.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/c/ppb_graphics_2d.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb_view.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_resource.h"

typedef enum {true, false}bool;
typedef bool gboolean;
typedef void * gpointer;
typedef int gint;
typedef unsigned int guint;

#define TRUE 1
#define FALSE 0
#define G_SIGNAL_NUM 10
#define GTK_WINDOW_TOPLEVEL 0
#define GTK_WINDOW
#define GTK_CONTAINER (GtkContainer *)
#define G_CALLBACK(handler) ((_g_signal_callback)(handler)) 

#define G_PI 3.1415926535
#define MIN(x,y) ((x)<(y))?(x):(y)
#define MAX(x,y) ((x)>(y))?(x):(y)

#define g_message printf

enum g_signal_id{
	DRAW
};
typedef enum{
	GDK_COLORSPACE_RGB
}GdkColorspace; 
typedef enum{
	GDK_INTERP_NEAREST
}GdkInterpType;
typedef struct _GError{
}GError;
typedef struct _GtkWidget GtkWidget;
typedef gboolean (* _g_signal_callback)	(GtkWidget *, cairo_t *, gpointer );
typedef struct _g_signal_node{
	_g_signal_callback g_signal_callback;	
}g_signal_node;
typedef struct _GdkWidget{
	cairo_surface_t *surface;
	PP_Resource image;
}GdkWidget;
typedef struct _GtkWidget{
	g_signal_node g_signal_list[G_SIGNAL_NUM];
	PP_Resource image;
	cairo_surface_t *surface;
	PP_Instance instance;
	int width, height, stride;
}GtkWidget;
typedef struct _GdkPixbuf{
	PP_Resource image;
	cairo_surface_t *surface;
	int width, height, stride;
}GdkPixbuf;
typedef struct _GdkRectangle{
	int x,y;
	int width,height;
}GdkRectangle;
typedef struct _GObject{
}GObject;
typedef GtkWidget GtkWindow;
typedef GtkWidget GtkContainer;
typedef gint (* GSourceFunc)(gpointer data);

PP_Module g_module_id;
PPB_GetInterface g_get_browser_interface;
PP_Instance CurInstance;
const PPB_ImageData *g_image_data_interface;
const PPB_Graphics2D *g_graphics_2d_interface;
const PPB_Instance* g_instance_interface;
const PPB_View* g_view_interface;
const PPB_Core* g_core_interface;

#define gdk_pixbuf_new_from_file(filename, error) \
	_gdk_pixbuf_new_from_file(CurInstance, filename, error)
GdkPixbuf *
_gdk_pixbuf_new_from_file (PP_Instance instance,
			   const char *filename, 
			   GError **error){
	cairo_surface_t *image_surface;
	PP_Resource image;
	unsigned char *surface_data;
	unsigned char *image_data;
	struct PP_ImageDataDesc image_desc;
	struct PP_Size size;
	int num_chars, i;
	GdkPixbuf *pixbuf = (GdkPixbuf *)malloc(sizeof(GdkPixbuf));
	image_surface = cairo_image_surface_create_from_png(filename);
	if(!image_surface){
		printf("cairo_image_surface_from file() error!\n");
		return NULL;
	}
	pixbuf->width = size.width = cairo_image_surface_get_width(image_surface);
	pixbuf->height = size.height = cairo_image_surface_get_height(image_surface);
	image = pixbuf->image = g_image_data_interface->Create(
		instance, PP_IMAGEDATAFORMAT_BGRA_PREMUL,&size,PP_TRUE);
	if(!image){
		printf("Image data create error!\n");
		return NULL;
	}
	g_image_data_interface->Describe(image, &image_desc);
	image_data = (unsigned char *)g_image_data_interface->Map(image);
	if(!image_data){
		g_core_interface->ReleaseResource(image);
		return NULL;
	}
	num_chars = image_desc.stride * size.height;
	pixbuf->stride = image_desc.stride;
	
	surface_data = cairo_image_surface_get_data(image_surface);
	memcpy(image_data, surface_data, num_chars*sizeof(char));
	pixbuf->surface = cairo_image_surface_create_for_data
		(image_data, CAIRO_FORMAT_ARGB32, 
		 pixbuf->width, pixbuf->height, pixbuf->stride);
	cairo_surface_destroy(image_surface);
	
	return pixbuf;
}
#define gdk_pixbuf_get_width(pixbuf) _gdk_pixbuf_get_width(pixbuf)
int _gdk_pixbuf_get_width(const GdkPixbuf *pixbuf){
	return pixbuf->width;
}
#define gdk_pixbuf_get_height(pixbuf) _gdk_pixbuf_get_height(pixbuf)
int _gdk_pixbuf_get_height(const GdkPixbuf *pixbuf){
	return pixbuf->height;
}
#define gdk_cairo_set_source_pixbuf(cr, pixbuf, pixbuf_x, pixbuf_y)\
	_gdk_cairo_set_source_pixbuf(cr, pixbuf, pixbuf_x, pixbuf_y)
void
_gdk_cairo_set_source_pixbuf (cairo_t         *cr,
                              const GdkPixbuf *pixbuf,
                              double          pixbuf_x,
                              double          pixbuf_y){
	cairo_set_source_surface(cr, pixbuf->surface, pixbuf_x, pixbuf_y);	
}

#define gdk_pixbuf_copy_area(src_pixbuf, src_x, src_y, width, height, dest_pixbuf, dest_x, dest_y)\
	_gdk_pixbuf_copy_area(src_pixbuf, src_x, src_y, width, height, dest_pixbuf, dest_x, dest_y)
void 
_gdk_pixbuf_copy_area (
		const GdkPixbuf *src_pixbuf, 
		int src_x, int src_y, 
		int width, int height, 
		GdkPixbuf *dest_pixbuf, int dest_x, int dest_y){
	unsigned char *src_image_data, *dest_image_data;
	struct PP_ImageDataDesc image_desc;
	int stride;
	int i,j;
	src_image_data = g_image_data_interface->Map(src_pixbuf->image);
	dest_image_data = g_image_data_interface->Map(dest_pixbuf->image);
	g_image_data_interface->Describe(src_pixbuf->image, &image_desc);
	stride = image_desc.stride;

	for(j=0; j<height; j++)
		for(i=0; i<width*4; i++){
			dest_image_data[(dest_y+j)*stride+dest_x+i] 
				= src_image_data[(src_y+j)*stride+src_x+i];	
		}

}
#define gdk_pixbuf_new(colorspace, has_alpha, bits_per_sample, width, height)\
	_gdk_pixbuf_new(Instance, colorspace, has_alpha, bits_per_sample, width, height)
GdkPixbuf * 
_gdk_pixbuf_new (PP_Instance instance,
		GdkColorspace colorspace, 
		gboolean has_alpha, 
		int bits_per_sample, 
		int width, 
		int height){
	GdkPixbuf *pixbuf;
	struct PP_Size size;
	unsigned char *image_data;
	struct PP_ImageDataDesc image_desc;
	
	size.width = width;
	size.height = height;
	
	pixbuf = (GdkPixbuf *)malloc(sizeof(GdkPixbuf));
	pixbuf->width = width;
	pixbuf->height = height;
	if(colorspace == GDK_COLORSPACE_RGB 
		&& has_alpha == FALSE 
		&& bits_per_sample == 8){
		pixbuf->image = g_image_data_interface->Create(
			CurInstance, PP_IMAGEDATAFORMAT_BGRA_PREMUL, &size,PP_TRUE);
		if(!(pixbuf->image)){
			printf("gdk_pixbuf_new(): Image data create error!\n");
			free(pixbuf);
			return NULL;
		}
		g_image_data_interface->Describe(pixbuf->image, &image_desc);
		pixbuf->stride = image_desc.stride;
		image_data = (unsigned char *)g_image_data_interface->Map(pixbuf->image);
		pixbuf->surface = cairo_image_surface_create_for_data
			(image_data, CAIRO_FORMAT_ARGB32, 
			 pixbuf->width, pixbuf->height, pixbuf->stride);
		
		return pixbuf;
	}

}

#define gdk_rectangle_intersect(src1, src2, dest)\
	_gdk_rectangle_intersect(src1, src2, dest)
gboolean
_gdk_rectangle_intersect (const GdkRectangle *src1,
			 const GdkRectangle *src2,
			 GdkRectangle       *dest)
{
  gint dest_x, dest_y;
  gint dest_x2, dest_y2;
  gint return_val;


  return_val = FALSE;

  dest_x = MAX (src1->x, src2->x);
  dest_y = MAX (src1->y, src2->y);
  dest_x2 = MIN (src1->x + src1->width, src2->x + src2->width);
  dest_y2 = MIN (src1->y + src1->height, src2->y + src2->height);

  if (dest_x2 > dest_x && dest_y2 > dest_y)
    {
      if (dest)
        {
          dest->x = dest_x;
          dest->y = dest_y;
          dest->width = dest_x2 - dest_x;
          dest->height = dest_y2 - dest_y;
        }
      return_val = TRUE;
    }
  else if (dest)
    {
      dest->width = 0;
      dest->height = 0;
    }

  return return_val;
}
#define gdk_pixbuf_composite(src, dest, dest_x, dest_y, dest_width, dest_height, offset_x, offset_y, scale_x, scale_y, interp_type, overall_alpha)\
	_gdk_pixbuf_composite(src, dest, dest_x, dest_y, dest_width, dest_height, offset_x, offset_y, scale_x, scale_y, interp_type, overall_alpha)

void 
_gdk_pixbuf_composite(
		const GdkPixbuf *src,
		GdkPixbuf *dest, 
		int dest_x, int dest_y, 
		int dest_width, int dest_height, 
		double offset_x, double offset_y, 
		double scale_x, double scale_y, 
		GdkInterpType interp_type, 
		int overall_alpha){
	cairo_t *dest_cr;
	int i,j;
	int stride;
	struct PP_Size size;
	stride = src->stride;
	dest_cr = cairo_create(dest->surface);

	cairo_translate(dest_cr, offset_x, offset_y);
	cairo_scale(dest_cr, scale_x, scale_y);
	cairo_set_source_surface(dest_cr, src->surface, dest_x-offset_x, dest_y-offset_y);

	cairo_paint_with_alpha(dest_cr, (overall_alpha/256.0));

}

gboolean isFlushed;

typedef struct _thread_timeout_func{
	guint interval;
	GSourceFunc function;
	gpointer data;
}thread_timeout_func;

void *_thread_timeout(void *func){
	thread_timeout_func *timeout_func = (thread_timeout_func *)func;
	gboolean result = TRUE;
	while(result){
		isFlushed = FALSE;
		usleep(timeout_func->interval);
		result = (*(timeout_func->function))(timeout_func->data);
		while(!isFlushed);
		printf("After while(!isFlushed)\n");
	}

}

#define gdk_threads_add_timeout(interval, function, data)\
	_gdk_threads_add_timeout(interval, function, data)
guint 
_gdk_threads_add_timeout (
		guint interval, 
		GSourceFunc function, 
		gpointer data)
{
	pthread_t pthread_timeout;
	thread_timeout_func *func = (thread_timeout_func *)malloc(sizeof(thread_timeout_func));
	func->interval = 1000*interval;
	func->function = function;
	func->data = data;
	pthread_create(&pthread_timeout, NULL, _thread_timeout, (void *)func);
	return pthread_timeout;
}


#define gtk_widget_show_all(widget) _gtk_widget_show_all(widget)
void _gtk_widget_show_all(GtkWidget *widget){

}



PP_Resource MakeAndBindGraphics(PP_Instance instance, struct PP_Size *size){
	PP_Resource graphics;
	graphics = g_graphics_2d_interface->Create(instance, size, PP_FALSE);
	if(!graphics){
		printf("Create graphics error!\n");
		return 0;
	}
	if(!g_instance_interface->BindGraphics(CurInstance, graphics)){
		printf("BindGraphics error!\n");
		g_core_interface->ReleaseResource(graphics);
		return 0;
	}
	return graphics;

}
PP_Resource graphics;
void FlushCompletionCallback(void *user_data, int32_t result){
	printf("Flush CompletionCallback() is called\n");
	isFlushed = TRUE;	
}
#define gtk_widget_queue_draw(widget)\
	_gtk_widget_queue_draw(widget)
void _gtk_widget_queue_draw(GtkWidget *widget){
	cairo_t *cr;
	struct PP_ImageDataDesc image_desc;
	unsigned char *surface_data, *image_data;
	int num_chars;
	struct PP_Size size;
	struct PP_Point point;
	struct PP_Rect rect;
	cr = cairo_create(widget->surface);
	
	widget->g_signal_list[DRAW].g_signal_callback(widget, cr, NULL);
	
	size.width = cairo_image_surface_get_width(widget->surface);
	size.height = cairo_image_surface_get_height(widget->surface);
	
	point.x = 0;
	point.y = 0;
	
	rect.point = point;
	rect.size = size;
	
	printf("size width:%d, height:%d\n", size.width, size.height);
	
	g_graphics_2d_interface->PaintImageData(graphics, widget->image, &point, &rect);
	g_graphics_2d_interface->Flush(graphics, PP_MakeCompletionCallback(&FlushCompletionCallback, NULL));
} 

#define gtk_main_quit _gtk_main_quit
void _gtk_main_quit (void){

}

#define gtk_main _gtk_main
void _gtk_main(){
	return;
}

#define gtk_init(argc, argv) _gtk_init(argc, argv)
void _gtk_init (int32_t *argc, const char **argv[]){

}

#define gtk_window_new(window_type)\
	_gtk_window_new(Instance, window_type)
GtkWidget * _gtk_window_new (PP_Instance instance, const uint32_t window_type){
	GtkWidget *widget;
	widget = (GtkWidget *)malloc(sizeof(GtkWidget));
	widget->instance = instance;
	widget->surface = NULL;
	widget->width = widget->height = 0;
	return widget;
}

#define gtk_widget_set_size_request(widget, a, b)\
	_gtk_widget_set_size_request(widget, a, b)
void _gtk_widget_set_size_request(GtkWidget* widget, gint a, gint b){
	struct PP_ImageDataDesc image_desc;
	unsigned char *image_data;
	struct PP_Size size;
	size.width = widget->width = a;
	size.height = widget->height = b;
	cairo_surface_destroy(widget->surface);
	widget->image = g_image_data_interface->Create
		(widget->instance, PP_IMAGEDATAFORMAT_BGRA_PREMUL, &size, PP_TRUE);
	if(!(widget->image)){
		printf("Image data create error!\n");
		return;
	}
	g_image_data_interface->Describe(widget->image, &image_desc);
	image_data = (unsigned char *)g_image_data_interface->Map(widget->image);
	if(!image_data){
		g_core_interface->ReleaseResource(widget->image);
		return;
	}
	widget->stride = image_desc.stride;
	widget->surface = cairo_image_surface_create_for_data
		(image_data, CAIRO_FORMAT_ARGB32, widget->width, widget->height, widget->stride);
	
	graphics = MakeAndBindGraphics(widget->instance, &size);

}

#define gtk_drawing_area_new() _gtk_drawing_area_new(Instance)
GtkWidget *_gtk_drawing_area_new (PP_Instance instance){
	GtkWidget *widget = (GtkWidget *)malloc(sizeof(GtkWidget));
	widget->instance = instance;
	widget->surface = NULL;
	widget->width = widget->height = 0;
	return widget;
}

#define gtk_container_add(container, widget)\
	_gtk_container_add(container, widget)
void gtk_container_add(GtkContainer* container, GtkWidget* widget){
	
	widget->instance = container->instance;
	widget->surface = cairo_surface_reference(container->surface);
	widget->width = container->width;
	widget->height = container->height;
	widget->image = container->image;
}

#define gtk_window_set_resizable(window, resizable)\
	_gtk_window_set_resizable(window, resizable)
void _gtk_window_set_resizable (GtkWindow *window, gboolean resizable){
	
	
}

#define g_source_remove(id) _g_source_remove(id)
void _g_source_remove(uint32_t id){

}
uint32_t g_signal_id_lookup(const char *name){
	if(!strcmp(name, "draw"))
		return DRAW;
}
uint32_t _g_signal_parse_name(const char *name){
	return g_signal_id_lookup(name);
}

#define g_signal_connect(instance, detailed_signal, c_handler, data)\
	_g_signal_connect(instance, detailed_signal, c_handler, data)
void _g_signal_connect(GtkWidget *instance, const char *detailed_signal, 
		_g_signal_callback c_handler,void *data){
	uint32_t detailed_signal_id;
	detailed_signal_id = _g_signal_parse_name(detailed_signal);
	instance->g_signal_list[detailed_signal_id].g_signal_callback = c_handler;
}
//-----
//------------------------------------------------------------------


PP_EXPORT int32_t PPP_InitializeModule(PP_Module module_id,
                                       PPB_GetInterface get_browser_interface) {
  // Save the global module information for later.
  g_module_id = module_id;
  g_get_browser_interface = get_browser_interface;
  g_graphics_2d_interface = (const PPB_Graphics2D*)
	  get_browser_interface(PPB_GRAPHICS_2D_INTERFACE);
  g_core_interface = (const PPB_Core *)
	  get_browser_interface(PPB_CORE_INTERFACE);
  g_image_data_interface = (const PPB_ImageData*)
	  get_browser_interface(PPB_IMAGEDATA_INTERFACE);
  g_instance_interface = (const PPB_Instance *)
	  get_browser_interface(PPB_INSTANCE_INTERFACE);
  g_view_interface = (const PPB_View *)
	  get_browser_interface(PPB_VIEW_INTERFACE);
  return PP_OK;
}

PP_EXPORT void PPP_ShutdownModule() {
}

/*
static void
print_hello (GtkWidget *widget,gpointer   data){
	  g_print ("Hello World\n");
}
*/


#define FRAME_DELAY 50

#define BACKGROUND_NAME "background.png"

static const char *image_names[] = {
	"apple-red.png",
	"gnome-applets.png",
	"gnome-calendar.png",
	"gnome-foot.png",
	"gnome-gmush.png",
	"gnome-gimp.png",
	"gnome-gsame.png",
	"gnu-keys.png"
};

#define N_IMAGES (sizeof (image_names) / sizeof (image_names[0]))

/* Current frame */
static GdkPixbuf *frame;

/* Background image */
static GdkPixbuf *background;
static int back_width, back_height;

/* Images */
static GdkPixbuf *images[N_IMAGES];

/* Widgets */
static GtkWidget *da;




/* Loads the images for the demo and returns whether the operation succeeded */
static gboolean
load_pixbufs (void)
{
	int i;

        /* We pass NULL for the error return location, we don't care
         * about the error message.
         */
        
	background = gdk_pixbuf_new_from_file (BACKGROUND_NAME, NULL);
	if (!background){
		printf("Couldn't load background\n");
		return FALSE;
	}
	back_width = gdk_pixbuf_get_width (background);
	back_height = gdk_pixbuf_get_height (background);
	
	for (i = 0; i < N_IMAGES; i++) {
		images[i] = gdk_pixbuf_new_from_file (image_names[i], NULL);
		if (!images[i]){
			printf("Couldn't load images[%d]:%s\n",i,image_names[i]);
			return FALSE;
		}
	}

	return TRUE;
}
/* Expose callback for the drawing area */
static gboolean
draw_cb (GtkWidget *widget, cairo_t *cr, gpointer data)
{
	cairo_surface_t *surface;
        gdk_cairo_set_source_pixbuf (cr, frame, 0, 0);
        cairo_paint (cr);

	return TRUE;
}

#define CYCLE_LEN 60

static int frame_num;


/* Timeout handler to regenerate the frame */
static gint
timeout (gpointer data)
{
	double f;
	int i;
	double xmid, ymid;
	double radius;

	gdk_pixbuf_copy_area (background, 0, 0, back_width, back_height,
			      frame, 0, 0);

	f = (double) (frame_num % CYCLE_LEN) / CYCLE_LEN;

	xmid = back_width / 2.0;
	ymid = back_height / 2.0;

	radius = MIN (xmid, ymid) / 2.0;

	for (i = 0; i < N_IMAGES; i++) {
		double ang;
		int xpos, ypos;
		int iw, ih;
		double r;
		GdkRectangle r1, r2, dest;
		double k;

		ang = 2.0 * G_PI * (double) i / N_IMAGES - f * 2.0 * G_PI;

		iw = gdk_pixbuf_get_width (images[i]);
		ih = gdk_pixbuf_get_height (images[i]);

		r = radius + (radius / 3.0) * sin (f * 2.0 * G_PI);

		xpos = floor (xmid + r * cos (ang) - iw / 2.0 + 0.5);
		ypos = floor (ymid + r * sin (ang) - ih / 2.0 + 0.5);

		k = (i & 1) ? sin (f * 2.0 * G_PI) : cos (f * 2.0 * G_PI);
		k = 2.0 * k * k;
		k = MAX (0.25, k);

		r1.x = xpos;
		r1.y = ypos;
		r1.width = iw * k;
		r1.height = ih * k;

		r2.x = 0;
		r2.y = 0;
		r2.width = back_width;
		r2.height = back_height;

		if (gdk_rectangle_intersect (&r1, &r2, &dest))
			gdk_pixbuf_composite (images[i],
					      frame,
					      dest.x, dest.y,
					      dest.width, dest.height,
					      xpos, ypos,
					      k, k,
					      GDK_INTERP_NEAREST,
					      ((i & 1)
					       ? MAX (127, fabs (255 * sin (f * 2.0 * G_PI)))
					       : MAX (127, fabs (255 * cos (f * 2.0 * G_PI)))));
	}

	gtk_widget_queue_draw (da);

	frame_num++;
	return TRUE;
}

static guint timeout_id;

/* Destroy handler for the window */
static void
destroy_cb (GObject *object, gpointer data)
{
	g_source_remove (timeout_id);
	timeout_id = 0;

	gtk_main_quit ();
}

//-----

PP_Bool DidCreate(PP_Instance Instance, uint32_t argc, const char *argn[], const char *argv[]){
	//printf("This is an instance!\n");
	extern PP_Instance CurInstance;
	CurInstance = Instance;
	
	//-----
	GtkWidget *window;

	printf("Instance: %d", Instance);

	gtk_init (&argc, &argv);

	if (!load_pixbufs ()) {
		g_message ("main(): Could not load all the pixbufs!");
		return PP_FALSE;
	}
	printf("After load_pixbufs\n");
	frame = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, back_width, back_height);
	printf("After gdk_pixbuf_new\n");
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	printf("After gtk_widow_new\n");
	gtk_widget_set_size_request (window, back_width, back_height);
        gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

	g_signal_connect (window, "destroy",
			  G_CALLBACK (destroy_cb), NULL);
	printf("After g_signal_connect destroy\n");
	da = gtk_drawing_area_new ();
	
	g_signal_connect (da, "draw",
			  G_CALLBACK (draw_cb), NULL);
	printf("After g_signal_connect draw\n");
	gtk_container_add (GTK_CONTAINER (window), da);
	printf("After gtk_container_add\n");
	timeout_id = gdk_threads_add_timeout (FRAME_DELAY, timeout, NULL);
	printf("After gdk_threads_add_timeout\n");
	gtk_widget_show_all (window);
	gtk_main ();
	//-----
	
	return PP_TRUE;
}
void DidDestroy(PP_Instance instance){

}
void DidChangeView(PP_Instance instance, PP_Resource view){

}
void DidChangeFocus(PP_Instance instance, PP_Bool has_focus){

}
PP_Bool HandleDocumentLoad(PP_Instance instance, PP_Resource url_loader){
	return PP_TRUE;
}
static PPP_Instance instance={
	&DidCreate,
	&DidDestroy,
	&DidChangeView,
	&DidChangeFocus,
	&HandleDocumentLoad
};
PP_EXPORT const void* PPP_GetInterface(const char* interface_name) {
  // You will normally implement a getter for at least PPP_INSTANCE_INTERFACE
  // here.
  if(strcmp(interface_name, PPP_INSTANCE_INTERFACE)==0)
	  return &instance;
  return NULL;
}

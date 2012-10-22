#ifndef __STUB_PIXBUF_H__
#define __STUB_PIXBUF_H__

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cairo.h>
#include <math.h>

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
#define GDK_COLORSPACE_RGB 0
#define GTK_WINDOW_TOPLEVEL
#define GTK_WINDOW
#define GTK_CONTAINER
#define GDK_INTERP_NEAREST 0
#define G_CALLBACK(handler) ((_g_signal_callback)(handler)) 

#define G_PI 3.1415926535
#define MIN(x,y) ((x)<(y))?(x):(y)
#define MAX(x,y) ((x)>(y))?(x):(y)


enum g_signal_id{
	DRAW
};

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
	GdkWidget window;
	PP_Instance instance;
}GtkWidget;
typedef struct _GdkPixbuf{
	PP_Resource image;
}GdkPixbuf;
typedef struct _GtkContainer{
}GtkContainer;
typedef struct _GdkRectangle{
	int x,y;
	int width,height;
}GdkRectangle;
typedef struct _GObject{
}GObject;
typedef GtkWidget GtkWindow;
typedef gint (* GSourceFunc)(gpointer data);

extern GtkWidget *gtk_drawing_area_new (void);
extern void gtk_widget_show_all(GtkWidget *);
extern void gtk_widget_set_size_request(GtkWidget*, gint, gint);
extern void gtk_container_add(GtkContainer*, GtkWidget*);
extern void gtk_widget_queue_draw(GtkWidget *widget);
extern void gtk_main_quit (void);
extern void gtk_main(void);
extern void gtk_init (uint32_t, const char *[]);
extern void gtk_window_set_resizable (GtkWindow *window, gboolean resizable){
extern GtkWidget * gtk_window_new (const uint32_t);

extern GdkPixbuf *_gdk_pixbuf_new_from_file (PP_Instance instance, const char *filename, GError **error);
#define gdk_pixbuf_new_from_file(filename, error) \
	_gdk_pixbuf_new_from_file(CurInstance, filename, error)
extern int gdk_pixbuf_get_width(const GdkPixbuf *pixbuf);
extern int gdk_pixbuf_get_height(const GdkPixbuf *pixbuf);
extern void gdk_cairo_set_source_pixbuf (cairo_t *cr, const GdkPixbuf *pixbuf, double pixbuf_x, double pixbuf_y);
extern void gdk_pixbuf_copy_area (const GdkPixbuf *src_pixbuf, int src_x, int src_y, int width, int height, GdkPixbuf *dest_pixbuf, int dest_x, int dest_y);
extern GdkPixbuf * gdk_pixbuf_new (GdkColorspace colorspace, gboolean has_alpha, int bits_per_sample, int width, int height);
extern gboolean gdk_rectangle_intersect (const GdkRectangle *src1, const GdkRectangle *src2, GdkRectangle *dest);
extern void gdk_pixbuf_composite (const GdkPixbuf *src, GdkPixbuf *dest, int dest_x, int dest_y, int dest_width, int dest_height, double offset_x, double offset_y, double scale_x, double scale_y, GdkInterpType interp_type, int overall_alpha);
extern guint gdk_threads_add_timeout (guint interval, GSourceFunc function, gpointer data);

PP_Module g_module_id;
PPB_GetInterface g_get_browser_interface = NULL;
PP_Instance CurInstance;
const PPB_ImageData *g_image_data_interface;
const PPB_Graphics2D *g_graphics_2d_interface;
const PPB_Instance* g_instance_interface;
const PPB_View* g_view_interface;
const PPB_Core* g_core_interface;
#endif

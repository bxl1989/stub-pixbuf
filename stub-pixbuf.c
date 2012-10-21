// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This is the simplest possible C Pepper plugin that does nothing. If you're
// using C++, you will want to look at stub.cc which uses the more convenient
// C++ wrappers.

#include "stub-pixbuf.h"


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

#define BACKGROUND_NAME "background.jpg"

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
       /* 
	background = gdk_pixbuf_new_from_file (BACKGROUND_NAME, NULL);
	if (!background){
		printf("Couldn't load background\n");
		return FALSE;
	}
	back_width = gdk_pixbuf_get_width (background);
	back_height = gdk_pixbuf_get_height (background);
	*/
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

uint32_t g_signal_id_lookup(const char *name){
	if(!strcmp(name, "draw"))
		return DRAW;
}
uint32_t g_signal_parse_name(const char *name){
	return g_signal_id_lookup(name);
}
#define g_signal_connect(instance, detailed_signal, c_handler, data)\
	_g_signal_connect(instance, detailed_signal, c_handler, data)
	
void _g_signal_connect(GtkWidget *instance, const char *detailed_signal, 
		_g_signal_callback c_handler,void *data){
	uint32_t detailed_signal_id;
	detailed_signal_id = g_signal_parse_name(detailed_signal);
	instance->g_signal_list[detailed_signal_id].g_signal_callback = c_handler;
}
//-----

PP_Bool DidCreate(PP_Instance Instance, uint32_t argc, const char *argn[], const char *argv[]){
	//printf("This is an instance!\n");
	extern PP_Instance CurInstance;

	int argcc=0;

	CurInstance = Instance;
	
	//-----
	GtkWidget *window;

	printf("Instance: %d", Instance);

	gtk_init (&argcc, NULL);

	if (!load_pixbufs ()) {
		g_message ("main(): Could not load all the pixbufs!");
		return PP_FALSE;
	}

	frame = gdk_pixbuf_new (GDK_COLORSPACE_RGB, FALSE, 8, back_width, back_height);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	gtk_widget_set_size_request (window, back_width, back_height);
        gtk_window_set_resizable (GTK_WINDOW (window), FALSE);

	g_signal_connect (window, "destroy",
			  G_CALLBACK (destroy_cb), NULL);

	da = gtk_drawing_area_new ();

	g_signal_connect (da, "draw",
			  G_CALLBACK (draw_cb), NULL);
	printf("After g_signal_connect() draw");
	gtk_container_add (GTK_CONTAINER (window), da);

	timeout_id = gdk_threads_add_timeout (FRAME_DELAY, timeout, NULL);

	gtk_widget_show_all (window);
	gtk_main ();
	//-----
	
	/*
	GtkWidget *window;
	GtkWidget *button;
	gtk_init (&argcc, NULL);
	
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	button=gtk_button_new_with_label("Hello");
	g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
	gtk_container_add(GTK_CONTAINER(window), button);
	gtk_widget_show(button);
	gtk_widget_show (window);
	
 	gtk_main ();
	*/
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

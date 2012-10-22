#include "stub-pixbuf.h"

void FlushCompletionCallback(void *user_data, int32_t result){
	//----Do Nothing----
}
GdkPixbuf *_gdk_pixbuf_new_from_file (PP_Instance instance, const char *filename, GError **error){
	cairo_surface_t *image_surface;
	PP_Resource image, graphics;
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
	size.width = cairo_image_surface_get_width(image_surface);
	size.height = cairo_image_surface_get_height(image_surface);
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
	surface_data = cairo_image_surface_get_data(image_surface);
	memcpy(image_data, surface_data, num_chars*sizeof(char));
	cairo_surface_destroy(image_surface);
	//---
	printf("CurInstance: %d\n", CurInstance);
	//---
	
	return pixbuf;
}

int gdk_pixbuf_get_width(const GdkPixbuf *pixbuf){
	struct PP_ImageDataDesc image_desc;
	g_image_data_interface->Describe(pixbuf->image, &image_desc);
	return image_desc.size.width;
}
int gdk_pixbuf_get_height(const GdkPixbuf *pixbuf){
	struct PP_ImageDataDesc image_desc;
	g_image_data_interface->Describe(pixbuf->image, &image_desc);
	return image_desc.size.height;
}
void
gdk_cairo_set_source_pixbuf (cairo_t         *cr,
                              const GdkPixbuf *pixbuf,
                              double          pixbuf_x,
                              double          pixbuf_y){
	unsigned char *image_data;
	cairo_surface_t *surface;
	int32_t width, height, stride;
	width = gdk_pixbuf_get_width(pixbuf);
	height = gdk_pixbuf_get_height(pixbuf);
	image_data = (unsigned char *)g_image_data_interface->Map(pixbuf->image);
	stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
	surface = cairo_image_surface_create_for_data(image_data, CAIRO_FORMAT_ARGB32, width, height, stride);
	cairo_set_source_surface(cr, surface, pixbuf_x, pixbuf_y);	
}

void 
gdk_pixbuf_copy_area (
		const GdkPixbuf *src_pixbuf, 
		int src_x, int src_y, 
		int width, int height, 
		GdkPixbuf *dest_pixbuf, int dest_x, int dest_y){
	unsigned char *src_image_data, *dest_image_data;
	src_image_data = g_image_data_interface->Map(src_pixbuf->image);
	dest_image_data = g_image_data_interface->Map(dest_pixbuf->image);
	

}

GdkPixbuf * 
gdk_pixbuf_new (GdkColorspace colorspace, 
		gboolean has_alpha, 
		int bits_per_sample, 
		int width, 
		int height){
	GdkPixbuf *pixbuf;
	PP_Size size;
	size.width = width;
	size.height = height;
	pixbuf = (GdkPixbuf *)malloc(sizeof(GdkPixbuf));
	if(colorspace == GDK_COLORSPACE_RGB 
		&& has_alpha == FALSE 
		&& bits_per_sample == 8){
		g_image_data_interface->Create(
			CurInstance, PP_IMAGEDATAFORMAT_BGRA_PREMUL,&size,PP_TRUE);
		if(!image){
			printf("Image data create error!\n");
			return NULL;
		}
	}

}
gboolean
gdk_rectangle_intersect (const GdkRectangle *src1,
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
void gdk_pixbuf_composite (const GdkPixbuf *src, GdkPixbuf *dest, int dest_x, int dest_y, int dest_width, int dest_height, double offset_x, double offset_y, double scale_x, double scale_y, GdkInterpType interp_type, int overall_alpha){

}
struct PP_CompletionCallback_Func_GSourceFunc_convertor{
	GSourceFunc function;
	PP_CompletionCallback callback;
}
PP_CompletionCallback  PP_CompletionCallback_Func_GSourceFunc_convert(struct PP_CompletionCallback_Func_GSourceFunc_convertor){
	
}
guint 
gdk_threads_add_timeout (
		guint interval, 
		GSourceFunc function, 
		gpointer data)
{
	int32_t result;
	struct PP_CompletionCallback callback;
	resutl = 0;
	callback.func = function;
	callback.user_data = data;
	callback.flags = PP_COMPLETIONCALLBACK_FLAG_NONE;
	while(1){
		g_core_interface->CallOnMainThread
			(interval, callback, result);
	}
}


GtkWidget *gtk_drawing_area_new (void){
	GtkWidget *widget = (GtkWidget *)malloc(sizeof(GtkWidget));
	widget->instance = CurInstance;
	return widget;
}
void gtk_widget_show_all(GtkWidget *widget){

}
void gtk_widget_set_size_request(GtkWidget* widget, gint a, gint b){

}
void gtk_container_add(GtkContainer* container, GtkWidget* widget){
}
void gtk_widget_queue_draw(GtkWidget *widget){
	cairo_t *cr;
	cairo_surface_t *surface;
	PP_Resource image;
	struct PP_ImageDataDesc image_desc;
	unsigned char *surface_data, *image_data;
	int num_chars;
	PP_Size size;
	widget->g_signal_list[DRAW].g_signal_callback(widget, cr, NULL);
	surface = cairo_get_target(cr);
	size.width = cairo_image_surface_get_width(surface);
	size.height = cairo_image_surface_get_height(surface);
	image = g_image_data_interface->Create(
		widget->instance, PP_IMAGEDATAFORMAT_BGRA_PREMUL,&size,PP_TRUE);
	g_image_data_interface->Describe(image, &image_desc);
	image_data = g_image_data_interface->Map(image);
	num_chars = image_desc.stride * size.height;
	surface_data = cairo_image_surface_get_data(surface);
	memcpy(image_data, surface_data, num_chars*sizeof(char));
	cairo_surface_destroy(image_surface);
	//---
	printf("CurInstance: %d\n", CurInstance);
	if(!(graphics = g_graphics_2d_interface->Create(CurInstance, &size, PP_FALSE))){
		printf("Create graphics error!\n");
		return NULL;
	}
	if(!g_instance_interface->BindGraphics(CurInstance, graphics)){
		printf("BindGraphics error!\n");
		return NULL;
	}
	g_graphics_2d_interface->ReplaceContents(graphics, image);
	g_graphics_2d_interface->Flush(graphics, PP_MakeCompletionCallback(&FlushCompletionCallback, NULL));
	//---
} 
void gtk_main_quit (void){

}
void gtk_main(){
	return;
}
void gtk_init (uint32_t argc, const char *argv[]){

}
GtkWidget * gtk_window_new (const uint32_t window_type){
	return NULL;
}
void gtk_window_set_resizable (GtkWindow *window, gboolean resizable){
}

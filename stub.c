// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This is the simplest possible C Pepper plugin that does nothing. If you're
// using C++, you will want to look at stub.cc which uses the more convenient
// C++ wrappers.

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <math.h>

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppp.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppp_instance.h"
PP_Module g_module_id;
PPB_GetInterface g_get_browser_interface = NULL;

PP_EXPORT int32_t PPP_InitializeModule(PP_Module module_id,
                                       PPB_GetInterface get_browser_interface) {
  // Save the global module information for later.
  g_module_id = module_id;
  g_get_browser_interface = get_browser_interface;

  return PP_OK;
}

PP_EXPORT void PPP_ShutdownModule() {
}
static void
print_hello (GtkWidget *widget,gpointer   data){
	  g_print ("Hello World\n");
}

PP_Bool DidCreate(PP_Instance Instance, uint32_t argc, const char *argn[], const char *argv[]){
	//printf("This is an instance!\n");
	int argcc=0;
	
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

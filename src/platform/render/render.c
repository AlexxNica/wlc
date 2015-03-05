#include <wlc/wlc.h>
#include <stdlib.h>
#include <assert.h>
#include "platform/context/context.h"
#include "render.h"
#include "gles2.h"

void
wlc_render_resolution(struct wlc_render *render, struct wlc_context *bound, const struct wlc_size *resolution)
{
   assert(render && bound && resolution);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.resolution)
      render->api.resolution(render->render, resolution);
}

void
wlc_render_surface_destroy(struct wlc_render *render, struct wlc_context *bound, struct wlc_surface *surface)
{
   assert(render && bound && surface);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.surface_destroy)
      render->api.surface_destroy(render->render, bound, surface);
}

bool
wlc_render_surface_attach(struct wlc_render *render, struct wlc_context *bound, struct wlc_surface *surface, struct wlc_buffer *buffer)
{
   assert(render && bound && surface);

   if (!wlc_context_bind(bound) || !render->api.surface_attach)
      return false;

   return render->api.surface_attach(render->render, bound, surface, buffer);
}

void
wlc_render_view_paint(struct wlc_render *render, struct wlc_context *bound, struct wlc_view *view)
{
   assert(render && view);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.view_paint)
      render->api.view_paint(render->render, view);
}

void
wlc_render_surface_paint(struct wlc_render *render, struct wlc_context *bound, struct wlc_surface *surface, struct wlc_origin *pos)
{
   assert(render);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.surface_paint)
      render->api.surface_paint(render->render, surface, pos);
}

void
wlc_render_pointer_paint(struct wlc_render *render, struct wlc_context *bound, struct wlc_origin *pos)
{
   assert(render);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.pointer_paint)
      render->api.pointer_paint(render->render, pos);
}

void
wlc_render_read_pixels(struct wlc_render *render, struct wlc_context *bound, struct wlc_geometry *geometry, void *out_data)
{
   assert(render);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.read_pixels)
      render->api.read_pixels(render->render, geometry, out_data);
}

void
wlc_render_background(struct wlc_render *render, struct wlc_context *bound)
{
   assert(render);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.background)
      render->api.background(render->render);
}

void
wlc_render_clear(struct wlc_render *render, struct wlc_context *bound)
{
   assert(render);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.clear)
      render->api.clear(render->render);
}

void
wlc_render_time(struct wlc_render *render, struct wlc_context *bound, uint32_t time)
{
   assert(render);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.time)
      render->api.time(render->render, time);
}

void
wlc_render_release(struct wlc_render *render, struct wlc_context *bound)
{
   assert(render);

   if (!wlc_context_bind(bound))
      return;

   if (render->api.terminate)
      render->api.terminate(render->render);

   memset(render, 0, sizeof(struct wlc_render));
}

bool
wlc_render(struct wlc_render *render, struct wlc_context *context)
{
   assert(render && context);
   memset(render, 0, sizeof(struct wlc_render));

   void* (*constructor[])(struct wlc_context*, struct wlc_render_api*) = {
      wlc_gles2,
      NULL
   };

   for (uint32_t i = 0; constructor[i]; ++i) {
      if ((render->render = constructor[i](context, &render->api)))
         return true;
   }

   wlc_log(WLC_LOG_WARN, "Could not initialize any rendering backend");
   wlc_render_release(render, context);
   return false;
}

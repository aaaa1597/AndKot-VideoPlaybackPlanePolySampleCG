package com.aaa.videoplaybackplanepolysamplecg

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.Surface

class GLVideoSurfaceView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null
) : GLSurfaceView(context, attrs) {
    private var renderer: GLVideoRenderer
    private var listener: ((Surface) -> Unit)? = null

    init {
        setEGLContextClientVersion(3)
        renderer = GLVideoRenderer { surfaceTexture ->
            val surface = Surface(surfaceTexture)
            listener?.invoke(surface)
        }
        setRenderer(renderer)
        renderMode = RENDERMODE_CONTINUOUSLY
    }

    fun setOnSurfaceReadyListener(l: (Surface) -> Unit) {
        listener = l
    }
}
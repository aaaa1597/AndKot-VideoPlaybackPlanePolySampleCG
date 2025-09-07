package com.aaa.videoplaybackplanepolysamplecg

import android.graphics.SurfaceTexture
import android.opengl.GLES11Ext
import android.opengl.GLES20
import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class GLVideoRenderer (
    private val onSurfaceReady: (SurfaceTexture) -> Unit
) : GLSurfaceView.Renderer {
    private var textureId: Int = 0
    private var surfaceTexture: SurfaceTexture? = null

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        textureId = createOESTexture()
        surfaceTexture = SurfaceTexture(textureId).apply {
            setOnFrameAvailableListener {
                // フレームが届いたら描画要求
            }
        }
        onSurfaceReady(surfaceTexture!!)
        nativeOnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        GLES20.glViewport(0, 0, width, height)
        nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        val mtx = FloatArray(16)
        surfaceTexture?.updateTexImage()
        surfaceTexture?.getTransformMatrix(mtx)
        nativeOnDrawFrame(textureId, mtx)
    }

    private fun createOESTexture(): Int {
        val tex = IntArray(1)
        GLES20.glGenTextures(1, tex, 0)
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, tex[0])
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR)
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR)
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE)
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE)
        return tex[0]
    }
}
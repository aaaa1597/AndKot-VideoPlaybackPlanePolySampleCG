package com.aaa.videoplaybackplanepolysamplecg

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.media3.exoplayer.ExoPlayer
import androidx.media3.common.MediaItem
import com.aaa.videoplaybackplanepolysamplecg.databinding.ActivityMainBinding
import androidx.core.net.toUri
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    private lateinit var _binding: ActivityMainBinding
    private lateinit var player: ExoPlayer

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        _binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(_binding.root)

        // ExoPlayer 初期化
        player = ExoPlayer.Builder(this).build()
        val mediaItem = MediaItem.fromUri("asset:///vuforiasizzlereel.m4v".toUri())
        player.setMediaItem(mediaItem)
        player.prepare()

        _binding.viwGlsurface.setOnSurfaceReadyListener { surface ->
            CoroutineScope(Dispatchers.Main).launch {
                player.setVideoSurface(surface)
                player.playWhenReady = true
            }
        }
    }

    override fun onPause() {
        super.onPause()
        player.pause()
        _binding.viwGlsurface.onPause()
    }

    override fun onResume() {
        super.onResume()
        _binding.viwGlsurface.onResume()
    }

    override fun onDestroy() {
        super.onDestroy()
        player.release()
    }

    companion object {
        // Used to load the 'videoplaybackplanepolysamplecg' library on application startup.
        init {
            System.loadLibrary("videoplaybackplanepolysamplecg")
        }
    }
}
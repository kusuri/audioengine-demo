package com.acme.androidaudioenginedemo;

import android.content.Context;
import android.net.Uri;
import android.view.ViewGroup;

import java.lang.ref.WeakReference;

public class DemoAudioEngine
{
    public interface Listener { void filePlaybackFinished(); }

    //==============================================================================
    static
    {
        System.loadLibrary("DemoAudioEngine");
    }

    //==============================================================================
    public DemoAudioEngine(Context context)
    {
        super();
        constructAudioEngine(context);
    }

    @Override
    public void finalize() throws java.lang.Throwable
    {
        setListener (null);
        destroyAudioEngine();
        super.finalize();
    }

    //==============================================================================
    public native void play(String uriToPlay);
    public native void stop();
    public native void pause();
    public native void resume();
    public native void setRoomSize(float roomSize);
    public native void setLowpassCutoff(float lowpassCutoff);
    public native void addWaveformComponentToNativeParentView (ViewGroup viewGroup);
    public native void removeWaveformComponentFromNativeParentView();

    //==============================================================================
    private native void constructAudioEngine(Context context);
    private native void destroyAudioEngine();

    //==============================================================================
    private void invokeFilePlaybackFinishedListener()
    {
        invokeListener();
    }

    //==============================================================================
    public void setListener(Listener listenerToAdd)
    {
        if (listenerToAdd != null)
            listener = new WeakReference<Listener>(listenerToAdd);
        else
            listener = null;
    }

    private void invokeListener()
    {
        if (listener != null)
        {
            Listener l = listener.get();

            if (l != null)
                l.filePlaybackFinished ();
        }
    }

    //==============================================================================
    private long cppCounterpartInstance = 0;
    private WeakReference<Listener> listener = null;
};


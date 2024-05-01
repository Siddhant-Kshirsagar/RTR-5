package com.sgk.window;

import android.content.Context;

import androidx.appcompat.widget.AppCompatTextView;

import android.graphics.Color;

import android.view.Gravity;

import android.view.MotionEvent;

import android.view.GestureDetector;

import android.view.GestureDetector.OnDoubleTapListener;

import android.view.GestureDetector.OnGestureListener;

public class MyTextView extends AppCompatTextView implements OnDoubleTapListener,OnGestureListener
{
    private GestureDetector gestureDetector;

    public MyTextView(Context context)
    {
        super(context);

        setTextColor(Color.rgb(0,255,0));
		setGravity(Gravity.CENTER);
		setTextSize(20);
		setText("Hello World !!!, Separate TextView");

        // create and set GestureDectector object 
        gestureDetector = new GestureDetector(context,this,null,false);
        gestureDetector.setOnDoubleTapListener(this);
    }

    // implementation of onTouchEvent of view class
    @Override
    public boolean onTouchEvent(MotionEvent e)
    {
        // code
        if(!gestureDetector.onTouchEvent(e))
        {
            super.onTouchEvent(e);
        }
        return (true);
    }


    // implementation of 3 methods of onDoubleTapListner interface

    @Override
    public boolean onDoubleTap(MotionEvent e)
    {
        // code
        setText("Double Tap");

        return(true);
    }

    @Override 
    public boolean onDoubleTapEvent(MotionEvent e)
    {
        // code
        return(true);
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e)
    {
        //code
        setText("Single Tap");
        return(true);
    }

    // implementation of 6 methods of onGestureListner interface

    @Override
    public boolean onDown(MotionEvent e)
    {
        //code
        return(true);
    }

    @Override
    public boolean onFling(MotionEvent e1,MotionEvent e2,float velocityX, float velocityY)
    {
        // code
        return(true);

    }

    @Override
    public void onLongPress(MotionEvent e)
    {
        // code
        setText("On Long Pressed");
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY)
    {
        // code
        setText("Scroll/Swipe");
        return(true);
    }

    @Override
    public void onShowPress(MotionEvent e)
    {
        // code
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e)
    {
        //code
        return(true);
    }
}




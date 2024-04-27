package com.sgk.window;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import androidx.appcompat.widget.AppCompatTextView;

import android.graphics.Color;

import android.view.Gravity;

//packages for full screen
import androidx.core.view.WindowCompat;

import androidx.core.view.WindowInsetsControllerCompat;

import androidx.core.view.WindowInsetsCompat;

// package for orientation
import android.content.pm.ActivityInfo; // pm - package manager

public class MainActivity extends AppCompatActivity
{
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);

		//hide action bar
		getSupportActionBar().hide();

		// do full screen
		WindowCompat.setDecorFitsSystemWindows(getWindow(),false);

		// get window insets controller object
		WindowInsetsControllerCompat windowInsetsControllerCompat = WindowCompat.getInsetsController(getWindow(),getWindow().getDecorView());

		// Tell above object to hide system bar means 1.status bar 2. navigation bar 3. caption bar and 4.input method editor(ime)
		windowInsetsControllerCompat.hide(WindowInsetsCompat.Type.systemBars()| WindowInsetsCompat.Type.ime());

		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

		//Setting background color to black
		getWindow().getDecorView().setBackgroundColor(Color.BLACK);

		AppCompatTextView myTextView = new AppCompatTextView(this);
		myTextView.setTextColor(Color.rgb(0,255,0));
		myTextView.setGravity(Gravity.CENTER);
		myTextView.setTextSize(40);
		myTextView.setText("Hello World !!!");

		setContentView(myTextView);

	}
}

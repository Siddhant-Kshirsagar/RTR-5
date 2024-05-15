package com.sgk.gles_perspective;

import android.os.Bundle;

import android.graphics.Color;

import androidx.appcompat.app.AppCompatActivity;

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

		GLESView glesView = new GLESView(this);


		setContentView(glesView);

	}

	@Override
	protected void onPause()
	{
		super.onPause();
	}

	@Override
	protected void onResume()
	{
		super.onResume();
	}
}

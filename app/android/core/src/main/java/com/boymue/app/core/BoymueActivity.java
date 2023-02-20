package com.boymue.app.core;

import android.app.Activity;
import android.os.Bundle;

import androidx.annotation.Nullable;

import com.boymue.app.core.view.BoymueView;

public class BoymueActivity extends Activity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new BoymueView(this));
    }

    public static class BoymueActivity1 extends BoymueActivity {}

    public static class BoymueActivity2 extends BoymueActivity {}

    public static class BoymueActivity3 extends BoymueActivity {}
}

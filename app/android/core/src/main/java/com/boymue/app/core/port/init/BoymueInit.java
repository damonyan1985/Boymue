package com.boymue.app.core.port.init;

import android.content.Context;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class BoymueInit {
    public static final String FONT_FILE = "NotoSansHans-Regular.otf";
    public static void initFont(Context context) {
        InputStream in = null;
        FileOutputStream out = null;
        String path = context.getApplicationContext().getFilesDir()
                .getAbsolutePath() + File.separator + FONT_FILE; // data/data目录

        File file = new File(path);
        if (!file.exists()) {
            try {
                in = context.getAssets().open(FONT_FILE);
                out = new FileOutputStream(file);
                int length = -1;
                byte[] buf = new byte[1024];
                while ((length = in.read(buf)) != -1) {
                    out.write(buf, 0, length);
                }
                out.flush();
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                if (in != null) {
                    try {
                        in.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
                if (out != null) {
                    try {
                        out.close();
                    } catch (IOException e1) {
                        e1.printStackTrace();
                    }
                }
            }
        }
    }
}

package com.boyia.app.shell.module

import androidx.fragment.app.Fragment

open class BaseFragment: Fragment() {
    open fun canPop() : Boolean {
        return true
    }

    open fun hide() {}
}
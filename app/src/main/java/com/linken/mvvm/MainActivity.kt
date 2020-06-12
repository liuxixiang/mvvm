package com.linken.mvvm

import android.graphics.Color
import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import com.mooc.common.utils.base.BaseActivity
import com.mooc.common.utils.widget.CommonToolBar

class MainActivity() : BaseActivity() {
    override val layoutId: Int
        get() = R.layout.activity_main

    override fun initCommonToolBar(commonToolBar: CommonToolBar) {
        var views = layoutInflater.inflate(R.layout.activity_text, null, false);
        commonToolBar.setTitle("hahah") {
//            it.setTextColor(Color.RED)
        }.setToolbar {
//            it.visibility =View.GONE

        }.setShadowGone(false)
            .setToolBarLeft { _, backView ->
                backView.setOnClickListener {
                    //重写左边点击时间
                    Toast.makeText(this, "hahaha", Toast.LENGTH_LONG).show()
                }
            }
            .setToolBarRightForTitle("hahahha") {
                Toast.makeText(this, "右边文本被点击", Toast.LENGTH_LONG).show()
            }.setToolBarRightForIcon(R.mipmap.common_ic_back) {
                Toast.makeText(this, "右边图标被点击", Toast.LENGTH_LONG).show()
            }
            .setToolBarLeft(
                customView = layoutInflater.inflate(
                    R.layout.activity_text,
                    null,
                    false
                )
            ) { toolbarLeftView: View, customView: View ->
                toolbarLeftView.setOnClickListener {
                    //重写左边点击时间
                    Toast.makeText(this, "自定义", Toast.LENGTH_LONG).show()
                }
            }.setToolBarRight(
                View.VISIBLE,
                customView = layoutInflater.inflate(
                    R.layout.activity_text,
                    null,
                    false
                )
            ) { view: View, _: ImageView?, _: TextView? ->
                view.setOnClickListener {
                    //重写右边点击时间
                    Toast.makeText(this, "自定义", Toast.LENGTH_LONG).show()
                }
            }

            .setCommonToolBarGone(true)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        commonToolBar.setTitle("ofoood") {
//            it.setTextColor(Color.BLUE)
        }
    }


}
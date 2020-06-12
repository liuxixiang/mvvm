package com.mooc.common.utils.base

import android.os.Bundle
import android.view.View
import android.view.ViewGroup
import androidx.appcompat.app.AppCompatActivity
import com.mooc.common.R
import com.mooc.common.utils.widget.CommonToolBar
import kotlinx.android.synthetic.main.common_activity_base.*
import kotlinx.android.synthetic.main.common_toolbar.*

abstract class BaseActivity() : AppCompatActivity() {
    /**
     * 获取布局文件
     */
    protected abstract val layoutId: Int
    open val backgroundColor = R.color.conmon_bg_color
    lateinit var commonToolBar: CommonToolBar

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
//        ARouter.getInstance().inject(this)
        // 添加标题栏布局
        addToolBarLayout()
    }

    /**
     * 添加标题栏布局
     */
    private fun addToolBarLayout() {
        if (layoutId == 0) return
        val view: View =
            layoutInflater.inflate(layoutId, this.window.decorView as ViewGroup, false)
        val viewGroup: View =
            layoutInflater.inflate(
                R.layout.common_activity_base,
                this.window.decorView as ViewGroup,
                false
            )

        // 设置背景颜色
        viewGroup.setBackgroundResource(backgroundColor)
        val commonBaseContent = viewGroup.findViewById<ViewGroup>(R.id.common_base_content)
        commonBaseContent.addView(view)
        setContentView(viewGroup)
        this.commonToolBar = CommonToolBar(toolbar).apply {
            initCommonToolBar(this)
        }
    }

    /**
     * 初始化toolbar
     */
    protected abstract fun initCommonToolBar(commonToolBar: CommonToolBar)
}
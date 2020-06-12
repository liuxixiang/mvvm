package com.mooc.common.utils.widget

import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.FrameLayout.LayoutParams
import android.widget.ImageView
import android.widget.TextView
import androidx.annotation.ColorInt
import androidx.annotation.DrawableRes
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import com.mooc.common.R

class CommonToolBar(toolbar: Toolbar) {
    private var toolbarLeft: ViewGroup = toolbar.findViewById(R.id.toolbar_left)
    private var toolbarMid: ViewGroup = toolbar.findViewById(R.id.toolbar_mid)
    private var toolbarRight: ViewGroup = toolbar.findViewById(R.id.toolbar_right)
    private var toolbar = toolbar

    companion object {
        private const val COMMON_TOOL_BAR_TITLE_TAG = "title_tag"
        const val COMMON_TOOL_BAR_RIGHT_TAG = "right_tag"
        const val COMMON_TOOL_BAR_LEFT_TAG = "left_tag"
    }

    init {
        (toolbar.context as AppCompatActivity).setSupportActionBar(toolbar)
        (toolbar.context as AppCompatActivity).supportActionBar?.setDisplayShowTitleEnabled(false)
        setToolBarLeft { _, addView ->
            addView.setOnClickListener {
                (toolbar.context as AppCompatActivity).finish()
            }
        }
    }


    /**
     * 显示阴影
     */
    fun setShadowGone(visible: Boolean): CommonToolBar {
        // 如果toolbar隐藏时,同时隐藏 toolbar 下面的 阴影
        val ivShadow = toolbar.findViewById<View>(R.id.common_base_iv_shadow)
        if (ivShadow != null) {
            ivShadow.visibility = if (visible) View.VISIBLE else View.GONE
        }
        return this
    }

    /**
     * 设置toolbar 属性
     */
    fun setToolbar(setToolbarView: (toolbar: Toolbar) -> Unit): CommonToolBar {
        setToolbarView(toolbar)
        return this
    }

    /**
     * 设置CommonToolBar 是否显示
     */
    fun setCommonToolBarGone(visible: Boolean): CommonToolBar {
        setToolbar {
            it.visibility = if (visible) View.VISIBLE else View.GONE
        }
        return this
    }

    /**
     * 设置标题
     */
    fun setTitle(
        title: String,
        setTitleView: ((titleView: TextView) -> Unit)? = null
    ): CommonToolBar {
        (toolbarMid.findViewWithTag(COMMON_TOOL_BAR_TITLE_TAG) ?: TextView(
            toolbar.context
        ).apply {
            this.tag = COMMON_TOOL_BAR_TITLE_TAG
            //设置默认参数
            gravity = Gravity.CENTER
            paint.isFakeBoldText = true
            toolbarMid.removeAllViews()
            toolbarMid.addView(
                this, LayoutParams(
                    LayoutParams.WRAP_CONTENT,
                    LayoutParams.MATCH_PARENT
                )
            )
        }).apply {
            text = title
            if (setTitleView != null) {
                setTitleView(this)
            }
        }
        return this
    }

    /**
     * 设置toolbar 左边布局
     */
    fun setToolBarLeft(
        visibility: Int = View.VISIBLE,
        customView: View? = null,
        setToolBarLeftView: ((toolbarLeftView: View, addView: View) -> Unit)? = null
    ): CommonToolBar {
        if (visibility != View.GONE) {
            (customView ?: toolbarLeft.findViewWithTag(COMMON_TOOL_BAR_LEFT_TAG)
            ?: LayoutInflater.from(toolbar.context)
                .inflate(R.layout.common_toolbar_left, null)).apply {
                toolbarLeft.removeAllViews()
                tag = COMMON_TOOL_BAR_LEFT_TAG
                toolbarLeft.addView(this)
                toolbarLeft.visibility = visibility
                //判断是不是添加默认
                val commonNavigateBack =
                    this.findViewById<ImageView>(R.id.common_navigate_back)
                setToolBarLeftView?.let { it(toolbarLeft, commonNavigateBack ?: this) }
            }
        }
        return this
    }

    /**
     * 设置toolbar 右边布局
     */
    fun setToolBarRight(
        visibility: Int = View.GONE,
        customView: View? = null,
        setToolBarRightView: ((toolbarRightView: View, iconView: ImageView?, titleView: TextView?) -> Unit)? = null
    ): CommonToolBar {
        if (visibility != View.GONE) {
            (customView ?: toolbarRight.findViewWithTag(COMMON_TOOL_BAR_RIGHT_TAG)
            ?: LayoutInflater.from(toolbar.context)
                .inflate(R.layout.common_toolbar_right, null)).apply {
                toolbarRight.removeAllViews()
                this.tag = COMMON_TOOL_BAR_RIGHT_TAG
                toolbarRight.addView(this)
                toolbarRight.visibility = visibility
                setToolBarRightView?.let {
                    it(
                        this,
                        findViewById(R.id.common_tool_bar_right_icon),
                        findViewById(R.id.common_tool_bar_right_title)
                    )
                }
            }
        }
        return this
    }

    /**
     * 设置ToolBarRight 只有文字
     */
    fun setToolBarRightForTitle(
        title: String,
        textSize: Float? = null,
        @ColorInt color: Int? = null,
        setToolBarRightListener: (() -> Unit)? = null
    ): CommonToolBar {
        setToolBarRight(View.VISIBLE) { _, iconView, titleView ->
            iconView?.visibility = View.GONE
            titleView?.visibility = View.VISIBLE
            titleView?.text = title
            textSize?.let {
                titleView?.textSize = it
            }
            color?.let {
                titleView?.setTextColor(it)
            }
            titleView?.setOnClickListener {
                setToolBarRightListener?.let { it1 -> it1() }
            }
        }
        return this
    }

    /**
     * 设置ToolBarRight 只有图片
     */
    fun setToolBarRightForIcon(
        @DrawableRes resId: Int,
        setToolBarRightListener: (() -> Unit)? = null
    ): CommonToolBar {
        setToolBarRight(View.VISIBLE) { _, iconView, titleView ->
            iconView?.visibility = View.VISIBLE
            titleView?.visibility = View.GONE
            iconView?.setImageResource(resId)
            iconView?.setOnClickListener {
                setToolBarRightListener?.let { it1 -> it1() }
            }
        }
        return this
    }


}
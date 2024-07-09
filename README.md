# WP-Comment-Reviewer | WordPress 待审评论管理器

> Author: [Vincy SHI](https://blog.vincy1230.net/)  
> Version: 0.1

This application is a remote comment reviewer for WordPress based on Qt. With this application, users can handle pending comments without going to the WordPress backend. This application interacts directly with the WordPress core through the WordPress REST API, without loading complex web files, helping small-scale source site owners to handle new comments conveniently and quickly anytime, anywhere.  
本应用是一款基于 Qt 开发的 WordPress 待审评论远程管理器。通过本应用，用户无需再前往 WordPress 后台即可处理待审评论。本应用通过 WordPress REST API 直接与 WordPress 内核交互，无需加载复杂的网页文件，助力小规模源站的站长随时随地方便快捷的处理新评论。

### Usage | 使用方式

1. Go to WordPress Dashboard -> Users -> Your Profile -> Application Passwords, generate a new application password  
   前往 WordPress 后台仪表板 -> 用户 -> 个人资料 -> 应用程序密码，生成一个新的应用程序密码

1. Run this application, input your WordPress site domain (without http:// or https:// ), admin email and the application password you just generated  
   运行本应用，输入 WordPress 站点域名 (不包含 http:// 或 https:// )、管理员登录邮箱和刚刚生成的应用程序密码

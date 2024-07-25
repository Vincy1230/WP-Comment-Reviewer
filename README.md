# WP-Comment-Reviewer | WordPress 待审评论管理器

> Author: [Vincy SHI](https://blog.vincy1230.net/)  
> Version: 0.1  
> Support Language: English | 简体中文 | 繁體中文 | ไทย (Automatically detects system language)  
> Beta version exe: [x64](https://github.com/Vincy1230/WP-Comment-Reviewer/releases/download/0.1-Beta.2/Boxed_Qt_6_6_3_MSVC2019_64bit.exe) | [x86_32](https://github.com/Vincy1230/WP-Comment-Reviewer/releases/download/0.1-Beta.2/Boxed_Qt_5_15_17_MSVC2019_32bit.exe) | [Releases](https://github.com/Vincy1230/WP-Comment-Reviewer/releases/tag/0.1-Beta.2)

This application is a remote comment reviewer for WordPress based on Qt. With this application, users can handle pending comments without going to the WordPress backend. This application interacts directly with the WordPress core through the WordPress REST API, without loading complex web files, helping small-scale source site owners to handle new comments conveniently and quickly anytime, anywhere.

本应用是一款基于 Qt 开发的 WordPress 待审评论远程管理器。通过本应用，用户无需再前往 WordPress 后台即可处理待审评论。本应用通过 WordPress REST API 直接与 WordPress 内核交互，无需加载复杂的网页文件，助力小规模源站的站长随时随地方便快捷的处理新评论。

> 简体中文文档：[Click here for Simplified Chinese document](https://blog.vincy1230.net/vincy1230/technology/c-cpp-qt/1201/)

Managing WordPress comments only requires three steps: open the software, click approve, close the software.

Therefore, I developed a Qt-based WordPress pending comment manager for high-frequency operations needed by actively maintained blogs: reviewing comments. It enables direct management upon opening, making it extremely convenient once you save your account and password.

## Instructions

1.  Go to WordPress Dashboard -> Users -> Profile -> Application Passwords, and generate a new application password.
2.  Run this application and fill in the login page:
    -   WordPress Domain: Enter the domain of your WordPress backend.
    -   Administrator Email: Enter the email of your administrator account.
    -   Application Password: Enter the application password you just generated (note this is not the backend login password).
3.  After logging in, the operations are roughly the same as the pending comment operations in the WordPress backend.

## FAQs

### Login Failed

Incorrect domain entry:

-   Only enter the domain where WordPress is located (based on your WordPress homepage domain), e.g., `blog.example.com`.
-   If your WordPress site is not on your domain's homepage, fill it in like this: `www.example.com/blog`.

Account does not have admin privileges:

-   Please use an account with admin privileges.
-   The criterion for this is `GET /wp/v2/settings`, which checks whether the account can read WordPress settings (if it can read, it is considered an admin).

Incorrect password:

-   Use the application password, not the traditional backend login account password.
-   If your WordPress version does not support application passwords, refer to the section at the end to modify the login method.

Not using https connection:

-   For security, WordPress requires an https connection for external application login.
-   If you indeed cannot use https, refer to the section at the end to modify the login method.

### Unable to Post Replies

The email account is a required field for WordPress comments. Therefore, if you did not use the administrator's email when logging into this program and used other valid login credentials (e.g., account ID), you will not be able to post replies, but other functions are unaffected.

## Modify Login Method

For security, WordPress only allows external login using application passwords under valid https connections. If you indeed cannot use https or obtain application passwords, you can bypass this restriction by installing the JSON Basic Authentication plugin.

**It is highly discouraged to use this method to log in, especially if both conditions cannot be met. You should prioritize fixing your site to comply with WordPress restrictions, otherwise your password may be exposed in plaintext on the internet. Do not use in production environments!**

1.  Visit the plugin homepage [Github@WP-API/Basic-Auth](https://github.com/WP-API/Basic-Auth) to download the plugin source code.
2.  Manually add the source code to your site's plugin directory (usually located at `/wp-content/plugins`).
3.  Visit the WordPress backend and activate JSON Basic Authentication in Plugins -> Installed Plugins.
4.  You can then bypass the WordPress restrictions and use the regular password or this application under non-https connections.

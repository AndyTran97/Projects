# Generated by Django 4.2.4 on 2023-10-21 06:37

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('grades', '0001_initial'),
    ]

    operations = [
        migrations.AlterField(
            model_name='submission',
            name='author',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='author_set', to=settings.AUTH_USER_MODEL),
        ),
    ]
